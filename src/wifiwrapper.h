#pragma once

#include "models\buffera.h"
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <HTTPClient.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266HTTPClient.h>
#endif


class WifiWrapper
{
private:
  BufferA _ssid;
  BufferA _password;
public:
    WifiWrapper();
    bool IsConnected();
    bool ConnectToAccessPoint(const char *ssid, const char *password);
    bool ReconnectToAccessPoint();
    ~WifiWrapper();
};

WifiWrapper::WifiWrapper(/* args */)
{
}

bool WifiWrapper::IsConnected()
{
  return WiFi.status() == WL_CONNECTED;
}

bool WifiWrapper::ConnectToAccessPoint(const char *ssid, const char *password)
{
  _ssid.Set(ssid);
  _password.Set(password);

  return ReconnectToAccessPoint();
}

bool WifiWrapper::ReconnectToAccessPoint()
{
  WiFi.mode(WIFI_OFF);
  WiFi.begin(_ssid.Get(), _password.Get());

  return IsConnected();
}

WifiWrapper::~WifiWrapper()
{
}

