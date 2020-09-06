#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#ifdef ESP32
    #include <HTTPClient.h>
#else
    #include <ESP8266HTTPClient.h>
#endif
#include <stdio.h>
#include "interfaces\icommunicator.h"

class Communicator : public ICommunicator
{
private:
    HTTPClient _client;
    String _baseUrl;
    String _barPadId;
    String _heartbeatJson;
    // Format bill/waiter requests payload
    const char *_requestFmt = "{\"tableid\":\"%s\",\"requestid\":\"%s\"}";
    char *_requestBuffer = nullptr;
public:
    Communicator(const char* baseUrl, const char* barPadId);
    virtual bool Heartbeat() override;
    virtual bool RequestWaiter(const char *requestId) override;
    virtual bool RequestBill(const char *requestId) override;
    virtual bool CancelAllRequests() override;
    ~Communicator();
};

Communicator::Communicator(const char* baseUrl, const char* barPadId)
{
    _baseUrl = baseUrl;
    _barPadId = barPadId;

    _heartbeatJson = "{\"tableid\":\"";
    _heartbeatJson.concat(barPadId);
    _heartbeatJson.concat("\"}");

    _client.setReuse(true);

    // Space to insert 2 UID - table (BarPad) and request
    _requestBuffer = new char[strlen(_requestFmt) + 2 * 32 + 1];
    // Serial.print("Heartbeat JSON: ");
    // Serial.println(_heartbeatJson);
}

bool Communicator::Heartbeat()
{
    // String url = _baseUrl + "heartbeat";
    const char url[] = "http://192.168.0.105:8079/api/v1/heartbeat";
    Serial.print("POSTing Heartbeat to: ");
    Serial.println(url);
    
    // _client.begin(url.c_str());
    _client.begin(url);
    ulong now = millis();
    ulong timeoustMs = 10000;
    while (_client.connected() == 0 && millis() - now < timeoustMs)
    {
        Serial.print(".");
        delay(50);
    }
    Serial.println("");
    _client.setTimeout(10000);
    int responseCode = _client.POST(_heartbeatJson);
    _client.end();

    // HTTPClient client;
    // client.begin("http://192.168.0.105:8079/api/v1/heartbeat");
    // ulong now = millis();
    // ulong timeoustMs = 10000;
    // while (client.connected() == 0 && millis() - now < timeoustMs)
    // {
    //     Serial.print(".");
    //     delay(50);
    // }
    // Serial.println("");
    
    // client.setTimeout(10000);
    // int responseCode = client.POST(_heartbeatJson);
    // client.end();
    Serial.print("Response code of heartbeat: ");
    Serial.println(responseCode);
    
    return responseCode == 200;
}

bool Communicator::RequestWaiter(const char *requestId)
{
    String url = _baseUrl + "request/waiter";
    sprintf(_requestBuffer, _requestFmt, _barPadId.c_str(), requestId);
    Serial.print("POSTing request for waiter to: ");
    Serial.println(url);
    Serial.println(_requestBuffer);
    _client.begin(url.c_str());
    _client.setTimeout(10000);

    int responseCode = _client.POST(_requestBuffer);
    Serial.print("Response code of request for waiter: ");
    Serial.println(responseCode);

    _client.end();
    
    return responseCode == 200;
}

bool Communicator::RequestBill(const char *requestId)
{
    Serial.println("Stub for bill request...");
    return false;
}

bool Communicator::CancelAllRequests()
{
    Serial.println("Stub for cancel all requests...");
    return false;
}


Communicator::~Communicator()
{
    if (_requestBuffer != nullptr)
    {
        delete []_requestBuffer;
    }
}


#endif
