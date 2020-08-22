#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "settings.h"
#include "credentials.h"
#include "httphandler.h"
#include "notifier.h"

// Possible WiFi states
const int WiFiClientInitializing = 0;
const int WiFiClientConnecting = 1;
const int WiFiSetup = 2;
const int WiFiServing = 4;

HttpHandler _handler;
Notifier _notifier;

int _currentState = WiFiClientInitializing;

void NotifyStatus();
void SetState(int state);

void setup()
{
  Serial.begin(9600);

  pinMode(PinRed, OUTPUT);
  pinMode(PinGreen, OUTPUT);
  pinMode(PinBlue, OUTPUT);
  pinMode(PinBuiltinLed, OUTPUT);
}

void loop()
{
  switch (_currentState)
  {
  case WiFiClientInitializing:
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    SetState(WiFiClientConnecting);
    break;

  case WiFiClientConnecting:
    if (WiFi.status() == WL_CONNECTED)
    {
      SetState(WiFiSetup);
    }
    break;

  case WiFiSetup:
    Serial.println("Going to start WEB server...");
    Serial.print("IP of BarPadClient: ");
    Serial.println(WiFi.localIP());
    _handler.Start(80);
    SetState(WiFiServing);
    break;

  default:
    break;
  }

  NotifyStatus();
}

void SetState(int state)
{
  _currentState = state;
  // _notifier.ResetTime();
}

void NotifyStatus()
{
  switch (_currentState)
  {
  case WiFiClientConnecting:
    _notifier.Notify("BB000000");
    break;
  case WiFiServing:
    _notifier.Notify("L0000000");
    break;
  default:
    _notifier.Notify("R0000000");
    break;
  }
}
