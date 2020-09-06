#ifndef ANALOGBUTTONS_H
#define ANALOGBUTTONS_H

#include <Arduino.h>

class AnalogButtons
{
private:
    unsigned char _inputPin;
    
    int _previousButton = -1;
    unsigned int _thresholdMs = 140;
    unsigned long _lastButtonPressed = 0;

    // 3.3V
    // int _maxAllowedValue = 1000;
    // int _firstMinValue = 155;
    // int _secondMinValue = 300;
    // int _thirdMinValue = 740;

    // 5V
    int _maxAllowedValue = 1030;
    int _firstMinValue = 215;
    int _secondMinValue = 415;
    int _thirdMinValue = 1020;

    bool IsButtonReallyPressed(int buttonToCheck);
    void DebugValue(int value);
public:
    AnalogButtons(unsigned char inputPin);
    int GetPressedButton();
};

AnalogButtons::AnalogButtons(unsigned char inputPin)
{
    _inputPin = inputPin;
    pinMode(_inputPin, INPUT_PULLUP);
}

int AnalogButtons::GetPressedButton()
{
    int value = analogRead(_inputPin);
    // DebugValue(value);
    if (value > _maxAllowedValue)
    {
        return 0;
    }
    if (value >= _thirdMinValue)
    {
        return IsButtonReallyPressed(3) ? 3 : 0;
    }
    if (value >= _secondMinValue)
    {
        return IsButtonReallyPressed(2) ? 2 : 0;
    }
    if (value >= _firstMinValue)
    {
        return IsButtonReallyPressed(1) ? 1 : 0;
    }

    return 0;
}

bool AnalogButtons::IsButtonReallyPressed(int buttonToCheck)
{
    if (buttonToCheck != _previousButton)
    {
        _previousButton = buttonToCheck;
        _lastButtonPressed = millis();
        return true;
    }
    if (millis() - _lastButtonPressed > _thresholdMs)
    {
        _previousButton = buttonToCheck;
        _lastButtonPressed = millis();
        return true;
    }

    return false;
}

void AnalogButtons::DebugValue(int value)
{
    static int previousValue = 0;
    if (abs(previousValue - value) > 10)
    {
        Serial.print("Read value: ");
        Serial.println(value);
        previousValue = value;
    }
}

#endif
