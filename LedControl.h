#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Arduino.h>

class LedControl {
public:
    LedControl(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);

    void setHue(int hue);
    void setSaturation(int saturation);
    void setBrightness(int brightness);
    void setIntensity(int intensity);
    void setCCT(int cct);
    void setPower(bool power);
    void update();

private:
    void hueToRGB(int h, int s, int v, int& r, int& g, int& b);
    void CCTToRGB(int kelvin, int& r, int& g, int& b);
    void multiplyBrightness(int brightness, int& r, int& g, int& b);
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;

    int hue;
    int saturation;
    int brightness;
    int intensity;
    int cct;
    bool power;
};

#endif // LEDCONTROL_H
