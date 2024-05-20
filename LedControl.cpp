#include "LedControl.h"

LedControl::LedControl(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
    : redPin(redPin), greenPin(greenPin), bluePin(bluePin),
      hue(0), saturation(100), brightness(100), intensity(100), cct(2700), power(false) {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}

void LedControl::setHue(int hue) {
    this->hue = hue;
    update();
}

void LedControl::setSaturation(int saturation) {
    this->saturation = saturation;
    update();
}

void LedControl::setBrightness(int brightness) {
    this->brightness = brightness;
    if (brightness < 1) {
        power = false;
    } else {
        power = true;
    }
    update();
}

void LedControl::setIntensity(int intensity) {
    this->intensity = intensity;
    update();
}

void LedControl::setCCT(int cct) {
    this->cct = cct;
    int r, g, b;
    CCTToRGB(cct, r, g, b);
    multiplyBrightness(brightness, r, g, b);
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
}

void LedControl::setPower(bool power) {
    this->power = power;
    update();
}

void LedControl::update() {
    if (!power) {
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 0);
        return;
    }

    int r, g, b;
    hueToRGB(hue, saturation, intensity, r, g, b);
    multiplyBrightness(brightness, r, g, b);
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
}

void LedControl::multiplyBrightness(int brightness, int& r, int& g, int& b) {
    float brightnessFactor = brightness / 100.0f;
    r = std::max(0, std::min(255, static_cast<int>(r * brightnessFactor)));
    g = std::max(0, std::min(255, static_cast<int>(g * brightnessFactor)));
    b = std::max(0, std::min(255, static_cast<int>(b * brightnessFactor)));
}

void LedControl::hueToRGB(int h, int s, int v, int& r, int& g, int& b) {
    float hf = h / 360.0f;
    float sf = s / 100.0f;
    float vf = v / 100.0f;

    int i = static_cast<int>(hf * 6);
    float f = hf * 6 - i;
    float p = vf * (1 - sf);
    float q = vf * (1 - f * sf);
    float t = vf * (1 - (1 - f) * sf);

    float rf, gf, bf;

    switch (i % 6) {
        case 0: rf = vf, gf = t, bf = p; break;
        case 1: rf = q, gf = vf, bf = p; break;
        case 2: rf = p, gf = vf, bf = t; break;
        case 3: rf = p, gf = q, bf = vf; break;
        case 4: rf = t, gf = p, bf = vf; break;
        case 5: rf = vf, gf = p, bf = q; break;
    }

    r = static_cast<int>(rf * 255);
    g = static_cast<int>(gf * 255);
    b = static_cast<int>(bf * 255);
}

void LedControl::CCTToRGB(int kelvin, int& r, int& g, int& b) {
    float temp = kelvin / 100.0F;
    float red, green, blue;

    if (temp <= 60) {
        red = 255;
    } else {
        red = temp - 60;
        red = 329.698727446 * pow(red, -0.1332047592);
        if (red < 0) red = 0;
        if (red > 255) red = 255;
    }

    if (temp <= 66) {
        green = temp;
        green = 99.4708025861 * log(green) - 161.1195681661;
        if (green < 0) green = 0;
        if (green > 255) green = 255;
    } else {
        green = temp - 60;
        green = 288.1221695283 * pow(green, -0.0755148492);
        if (green < 0) green = 0;
        if (green > 255) green = 255;
    }

    if (temp >= 66) {
        blue = 255;
    } else {
        if (temp <= 19) {
            blue = 0;
        } else {
            blue = temp - 10;
            blue = 138.5177312231 * log(blue) - 305.0447927307;
            if (blue < 0) blue = 0;
            if (blue > 255) blue = 255;
        }
    }
    r = static_cast<int>(red);
    g = static_cast<int>(green);
    b = static_cast<int>(blue);
}
