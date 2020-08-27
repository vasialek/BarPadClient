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
#include "communicator.h"

// Possible WiFi states
const int WiFiClientInitializing = 0;
const int WiFiClientConnecting = 1;
const int WiFiSetup = 2;
const int WiFiServing = 4;

HttpHandler _handler;
Notifier _notifier;
Communicator _communicator("http://192.168.0.100:8079/api/v1/", "TableId_1234567890");

int _currentState = WiFiClientInitializing;

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

    // TestGetApi();
    // Test();
    // _communicator.Heartbeat();
    break;

  default:
    break;
  }

  static ulong lastHeartbeatAt = 0;
  if (lastHeartbeatAt == 0 || millis() > lastHeartbeatAt + 30000)
  {
    _communicator.Heartbeat();
    lastHeartbeatAt = millis();
  }
  

  NotifyStatus();
}

void Test()
{
  HTTPClient http;
  String serverName = "http://192.168.0.100:8079/api/v1/";
  int responseCode;
  String response;
  
  String url = serverName + "post";
  Serial.print("Sending POST request to: ");
  Serial.println(url);
  http.begin(url.c_str());

  // int responseCode = http.GET();
  responseCode = http.POST("{\"tableid\":\"1234567890\"}");

  Serial.print("HTTP response code: ");
  Serial.println(responseCode);

  if (responseCode > 0)
  {
    String response = http.getString();
    Serial.println(response);
  }

  http.end();
}

void TestGetApi()
{
  // Serial.println("Setting time via NTP...");
  // // Set time via NTP, as required for x.509 validation
  // configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // Serial.print("Waiting for NTP time sync: ");
  // time_t now = time(nullptr);
  // while (now < 8 * 3600 * 2) {
  //   delay(500);
  //   Serial.print(".");
  //   now = time(nullptr);
  // }
  // Serial.println("");
  // struct tm timeinfo;
  // gmtime_r(&now, &timeinfo);
  // Serial.print("Current time: ");
  // Serial.print(asctime(&timeinfo));

  const char* host = "192.168.0.100";
  WiFiClient client;
  Serial.print("Connecting to ");
  Serial.println(host);

  // Serial.printf("Using certificate: %s\n", trustRoot);
  // client.setTrustAnchors(&cert);

  const int port = 8079;
  if (!client.connect(host, port)) {
    Serial.println("Connection failed");
    return;
  }

  // String url = "/api/v1/version";
  String url = "/";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Buffer size, 128 bytes in this case
  const int bufferSize = 128;
  // Pointer to actual buffer
  uint8_t * buffer = new uint8_t[bufferSize];
  // String to hold the final response
  String _responseString = "";
  Serial.println("Response:");
  while (client.available())
  {
    char ch = client.read();
    Serial.print(ch);
    // int received = client.read(buffer, bufferSize);
    // if (received <= 0)
    // {
    //   Serial.println("Error reading from socket!");
    //   return;
    // }
    
    // _responseString += String((char*)buffer).substring(0, received);
  }
  delete[] buffer;
  Serial.println("Got response from server:");
  // Serial.println(_responseString);

  // Serial.println("Request sent");
  // while (client.connected()) {
  //   String line = client.readStringUntil('\n');
  //   if (line == "\r") {
  //     Serial.println("Headers received");
  //     break;
  //   }
  // }
  // String line = client.readStringUntil('\n');
  // if (line.startsWith("{\"state\":\"success\"")) {
  //   Serial.println("esp8266/Arduino CI successfull!");
  // } else {
  //   Serial.println("esp8266/Arduino CI has failed");
  // }
  // Serial.println("Reply was:");
  // Serial.println("==========");
  // Serial.println(line);
  // Serial.println("==========");
  // Serial.println("Closing connection");
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
