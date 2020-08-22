#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "Arduino.h"
#include "settings.h"

class Notifier
{
private:
    const static int StateDelay = 0;
    const static int StateNotifyOn = 1;
    const static int StateNotifyOff = 2;

    unsigned long _lastHeartbeatAt = 0;
    unsigned long _lastHeartbeatTickAt = 0;
    unsigned long _heartbeatTimeoutMs = 10000;
    unsigned long _heartbeatTickTimeoutMs = 1000;

    int _currentState;
    int _heartbeatTick = 0;  // will inform about each 8 symbols of state
    void TurnOnRedStateLed(){ digitalWrite(PinRed, HIGH); }
    void TurnOffRedStateLed(){ digitalWrite(PinRed, LOW); }
    void TurnOnGreenStateLed(){ digitalWrite(PinGreen, HIGH); }
    void TurnOffGreenStateLed(){ digitalWrite(PinGreen, LOW); }
    void TurnOnBlueStateLed(){ digitalWrite(PinBlue, HIGH); }
    void TurnOffBlueStateLed(){ digitalWrite(PinBlue, LOW); }
    void TurnOnBuiltinStateLed(){ digitalWrite(PinBuiltinLed, HIGH); }
    void TurnOffBuiltinStateLed(){ digitalWrite(PinBuiltinLed, LOW); }
public:
    Notifier()
    {
        _currentState = StateDelay;
    }
    // void ResetTime();
    void Notify(const char* state);
    void TurnOnStateTick(const char c);
    void TurnOffStateTick(const char c);
    // ~Notifier();
};

// void Notifier::ResetTime()
// {
//     _lastHeartbeatAt = 0;
//     _lastHeartbeatTickAt = 0;
//    _heartbeatTick = 0;
// }

void Notifier::Notify(const char* state)
{
    switch (_currentState)
    {
    case StateDelay:
        if (_lastHeartbeatAt == 0 || millis() > _lastHeartbeatAt + _heartbeatTimeoutMs)
        {
            _currentState = StateNotifyOn;
        }
        break;
    
    case StateNotifyOn:
        // std::cout << _heartbeatTick++ << std::endl;

        if (_lastHeartbeatTickAt == 0 || millis() > _lastHeartbeatTickAt + _heartbeatTickTimeoutMs)
        {
            TurnOnStateTick(state[_heartbeatTick]);
            _currentState = StateNotifyOff;
            // _heartbeatTick++;
            _lastHeartbeatTickAt = millis();
        }

        // // Loop through 8 symbols of state
        // if (_heartbeatTick > 7)
        // {
        //     std::cout << std::endl;
        //     _currentState = StateDelay;
        //     _heartbeatTick = 0;
        //     _lastHeartbeatAt = millis();
        // }
        break;
    case StateNotifyOff:
        if (millis() > _lastHeartbeatTickAt + _heartbeatTickTimeoutMs)
        {
            TurnOffStateTick(state[_heartbeatTick]);
            _heartbeatTick++;
            // std::cout << "Tick #" << _heartbeatTick << std::endl;
            _lastHeartbeatTickAt = millis();
            _currentState = StateNotifyOn;
        }

        if (_heartbeatTick > 7)
        {
            _heartbeatTick = 0;
            _lastHeartbeatAt = millis();  // wait after last notification
            _currentState = StateDelay;
        }
        
        
        break;
    }
}

void Notifier::TurnOnStateTick(const char c)
{
    // Serial.println("TurnOnStateTick...");
    // digitalWrite(PinGreen, HIGH);
    switch (c)
    {
    case 'R':
    case 'r':
        TurnOnRedStateLed();
        break;
    case 'G':
    case 'g':
        TurnOnGreenStateLed();
        break;
    case 'B':
    case 'b':
        TurnOnBlueStateLed();
        break;
    case 'L':
    case 'l':
        TurnOnBuiltinStateLed();
        break;
    }
}

void Notifier::TurnOffStateTick(const char c)
{
    // Serial.println("TurnOffStateTick...");
    // digitalWrite(PinGreen, LOW);
    switch (c)
    {
    case 'R':
    case 'r':
        TurnOffRedStateLed();
        break;
    case 'G':
    case 'g':
        TurnOffGreenStateLed();
        break;
    case 'B':
    case 'b':
        TurnOffBlueStateLed();
        break;
    case 'L':
    case 'l':
        TurnOffBuiltinStateLed();
        break;
    }
}

// Notifier::Notifier(/* args */)
// {
// }

// Notifier::~Notifier()
// {
// }

#endif
