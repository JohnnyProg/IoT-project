#include "SensorControl.h"

// SensorControl::SensorControl() {

// }

void SensorControl::setupSensor() {
  Serial.println("Connecting to sensor");
  delay(3000);
  bool status;
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x77);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void SensorControl::serialPrint() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}

void SensorControl::getData(float& t, float& h, float& p) {
  float ht = bme.readHumidity();
  float tt = bme.readTemperature();
  float pt = bme.readPressure() / 100.0F;
  t = roundToTwoDecimalPlaces(tt);
  h = roundToTwoDecimalPlaces(ht);
  p = roundToTwoDecimalPlaces(pt);
}

float SensorControl::roundToTwoDecimalPlaces(float val) {
  return round(val * 100.0) / 100.0;
}
