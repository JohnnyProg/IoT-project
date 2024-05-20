#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H
//sensor libraries
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)

class SensorControl {
public:
  Adafruit_BME280 bme;
  // SensorControl();
  void setupSensor();
  void serialPrint();
  void getData(float& t, float& h, float& p);
private:
  float roundToTwoDecimalPlaces(float val);
};

#endif