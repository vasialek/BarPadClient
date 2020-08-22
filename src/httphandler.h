#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "logheler.h"

class HttpHandler
{
private:
    LogHelper _logger;
    AsyncWebServer* _server = nullptr;
    void SetupHandlers();
public:
    HttpHandler();
    void Start(int port);
    ~HttpHandler();
};

HttpHandler::HttpHandler()
{
}

void HttpHandler::Start(int port)
{
    _logger.Info("Starting WebServer on %d port...", port);
    _server = new AsyncWebServer(port);
    SetupHandlers();
    _server->begin();
}

void HttpHandler::SetupHandlers()
{
    _server->on("/api/v1/reset", HTTP_GET, [](AsyncWebServerRequest* request){
        request->send(200);
    });
    _server->onNotFound([](AsyncWebServerRequest* request){
        request->send(404);
    });
}

HttpHandler::~HttpHandler()
{
}


#endif
