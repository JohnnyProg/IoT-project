#include <algorithm>
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <wifi_provisioning/manager.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_schedule.h>
#include <esp_rmaker_scenes.h>
// Set Defalt Values
#define DEFAULT_RELAY_MODE true
#define DEFAULT_Temperature 0
#define DEFAULT_Humidity 0

#include "LedControl.h"
#include "SensorControl.h"
#include <SimpleTimer.h>

SimpleTimer timer;
// BLE Credentils
const char *service_name = "PROV_12345";
const char *pop = "1234567";

// GPIO
static uint8_t gpio_reset = 0;
static uint8_t DHTPIN = 19;

LedControl ledControl(25, 26, 27);

SensorControl sensorControl;

bool wifi_connected = 0;

//------------------------------------------- Declaring Devices -----------------------------------------------------//

static TemperatureSensor temperature("Temperature", NULL, 25);
static TemperatureSensor humidity("Humidity", NULL, 70);
static TemperatureSensor pressure("Pressure", NULL, 1000);
static LightBulb lightbulb("Swiatlo");

void sysProvEvent(arduino_event_t *sys_event)
{
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
      printQR(service_name, pop, "ble");
#else
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
      printQR(service_name, pop, "softap");
#endif
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.printf("\nConnected to Wi-Fi!\n");
      wifi_connected = 1;
      delay(500);
      break;
    case ARDUINO_EVENT_PROV_CRED_RECV: {
        Serial.println("\nReceived Wi-Fi credentials");
        Serial.print("\tSSID : ");
        Serial.println((const char *) sys_event->event_info.prov_cred_recv.ssid);
        Serial.print("\tPassword : ");
        Serial.println((char const *) sys_event->event_info.prov_cred_recv.password);
        break;
      }
    case ARDUINO_EVENT_PROV_INIT:
      wifi_prov_mgr_disable_auto_stop(10000);
      break;
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      Serial.println("Stopping Provisioning!!!");
      wifi_prov_mgr_stop_provisioning();
      break;
  }
}
void led_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
  const char *device_name = device->getDeviceName();
  Serial.println(device_name);
  const char *param_name = param->getParamName();
  Serial.println(param_name);
  Serial.println(val.val.i);
  if (strcmp(param_name, "CCT") == 0) {
    ledControl.setCCT(val.val.i);
  } else if (strcmp(param_name, "Power") == 0) {
    ledControl.setPower(val.val.i);
  } else if (strcmp(param_name, "Hue") == 0) {
    ledControl.setHue(val.val.i);
  } else if (strcmp(param_name, "Saturation") == 0) {
    ledControl.setSaturation(val.val.i);
  } else if (strcmp(param_name, "Brightness") == 0) {
    ledControl.setBrightness(val.val.i);
  } else if (strcmp(param_name, "Intensity") == 0) {
    ledControl.setIntensity(val.val.i);
  }
  param->updateAndReport(val);
}


void setup()
{
  timer.setInterval(60000);
  Serial.begin(115200);
   sensorControl.setupSensor();
  pinMode(gpio_reset, INPUT);

  //------------------------------------------- Declaring Node -----------------------------------------------------//
  Node my_node;
  my_node = RMaker.initNode("Techiesms");

  lightbulb.addCb(led_callback);
  lightbulb.addBrightnessParam(100);
  lightbulb.addHueParam(30);
  lightbulb.addSaturationParam(30);
  lightbulb.addIntensityParam(30);
  lightbulb.addCCTParam(1);

  //------------------------------------------- Adding Devices in Node -----------------------------------------------------//
  my_node.addDevice(temperature);
  my_node.addDevice(humidity);
  my_node.addDevice(pressure);
  my_node.addDevice(lightbulb);

  RMaker.enableTZService();
  RMaker.enableSchedule();
  RMaker.enableScenes();
  RMaker.setTimeZone("Europe/Warsaw");
  Serial.printf("\nStarting ESP-RainMaker\n");
  RMaker.start();

  WiFi.onEvent(sysProvEvent);

#if CONFIG_IDF_TARGET_ESP32
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#else
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);g
#endif

}


void loop()
{
  if (wifi_connected && timer.isReady()) {                    // Check is ready a second timer                        // Reset a second timer
    Send_Sensor();
    timer.reset();
  }
  //-----------------------------------------------------------  Logic to Reset RainMaker

  // Read GPIO0 (external button to reset device
  if (digitalRead(gpio_reset) == LOW) { //Push button pressed
    Serial.printf("Reset Button Pressed!\n");
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while (digitalRead(gpio_reset) == LOW) delay(50);
    int endTime = millis();

    if ((endTime - startTime) > 10000) {
      // If key pressed for more than 10secs, reset all
      Serial.printf("Reset to factory.\n");
      wifi_connected = 0;
      RMakerFactoryReset(2);
    } else if ((endTime - startTime) > 3000) {
      Serial.printf("Reset Wi-Fi.\n");
      wifi_connected = 0;
      // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
      RMakerWiFiReset(2);
    }
  }
  delay(100);
}

void Send_Sensor()
{
  // sensorControl.serialPrint();
  Serial.print("wysyłam dane i mam z tego wielka radosc");
  float t, h, p;
  sensorControl.getData(t, h, p);
  temperature.updateAndReportParam("Temperature", t);
  humidity.updateAndReportParam("Temperature", h);
  pressure.updateAndReportParam("Temperature", p);
}
