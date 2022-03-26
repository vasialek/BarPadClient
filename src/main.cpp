#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <HTTPClient.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266HTTPClient.h>
#endif
#include <ESPAsyncWebServer.h>
// #include <WiFiClientSecure.h>
#include <WiFiClient.h>

#include "settings.h"
#include "credentials.h"
#include "httphandler.h"
#include "notifier.h"
#include "idgenerator.h"
#include "communicator.h"
#include "requestsender.h"
#include "userrequestsrepository.h"
#include "analogbuttons.h"
#include "requestbuilder.h"
#include "datetimeprovider.h"

// Possible WiFi states
const int WiFiClientInitializing = 0;
const int WiFiClientConnecting = 1;
const int WiFiSetup = 2;
const int WiFiServing = 4;
const int WiFiConfiguration = 8;

HttpHandler _handler;
Notifier _notifier;
DateTimeProvider _dateTimeProvider;
RequestBuilder requestBuilder;
Communicator _communicator(&requestBuilder, "192.168.0.102", "/api/v1/", 8079);
RequestSender _requestSender(&_communicator, &_dateTimeProvider);
IdGenerator generator;
UserRequestsRepository _requestsRepository(TableId, &_requestSender, &generator);
AnalogButtons _buttons(A0);

int _currentState = WiFiClientInitializing;

bool _doSomeTestThing = true;

void NotifyStatus();
void SetState(int state);
void TestGetApi();
void Test();

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
  int buttonPressed = 0;
  switch (_currentState)
  {
  case WiFiClientInitializing:
    WiFi.mode(WIFI_OFF);
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

  case WiFiServing:
    buttonPressed = _buttons.GetPressedButton();

    if (Serial.available() > 0)
    {
      String data = Serial.readString();
      Serial.print("Got command from Serial: ");
      Serial.println(data);
    }
    

    // if (_doSomeTestThing)
    // {
    //   buttonPressed = 1;
    //   _doSomeTestThing = false;
    // }

    if (buttonPressed > 0)
    {
      Serial.print("Got pressed button: ");
      Serial.println(buttonPressed);
      switch (buttonPressed)
      {
      case 1:
        Serial.print("Waiter is called: ");
        Serial.println(_requestsRepository.SetWaiterIsRequested());
        break;
      case 2:
        Serial.print("Bill is requested: ");
        Serial.println(_requestsRepository.SetBillIsRequested());
        break;
      case 3:
        Serial.println("All requests are canceled.");
        _requestsRepository.CancelAllRequest();
        break;
      default:
        break;
      }
    }

    _requestSender.ProcessRequests(DelayBeforeCalllRetriesMs);
    break;
    

  default:
    break;
  }

  static ulong lastHeartbeatAt = 0;
  if (lastHeartbeatAt == 0 || millis() > lastHeartbeatAt + 30000)
  {
    _communicator.Heartbeat(TableId);
    lastHeartbeatAt = millis();
  }
  

  // NotifyStatus();
}

void SetState(int state)
{
  if (_currentState != state)
  {
    Serial.print("New state is: ");
    Serial.println(state);
  }
  
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
