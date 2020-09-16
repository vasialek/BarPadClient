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
    const ulong TimeoutMs = 10000;
    HTTPClient _client;
    String _host;
    String _uri;
    int _port;

    String _heartbeatUri;
    String _requestWaiterUri;
    String _requestBillUri;

    String _barPadId;
    String _heartbeatJson;
    // Format bill/waiter requests payload
    const char *_requestFmt = "{\"tableid\":\"%s\",\"requestid\":\"%s\"}";
    char *_requestBuffer = nullptr;

    bool IsHttpClientConnected(HTTPClient *client, ulong timeoutMs);
    void DebugRequestUri(const char *msg, const char *host, const char *uri, int port);

public:
    Communicator(String host, String uri, int port, const char *barPadId);
    virtual bool Heartbeat(const char *tableId) override;
    virtual bool RequestWaiter(const char *tableId, const char *requestId) override;
    virtual bool RequestBill(const char *tableId, const char *requestId) override;
    virtual bool CancelAllRequests(const char *tableId) override;
    ~Communicator();
};

Communicator::Communicator(String host, String uri, int port, const char *barPadId)
{
    _client.setTimeout(TimeoutMs);
    _host = host;
    _uri = uri;
    _port = port;

    _heartbeatUri = String("/api/v1/heartbeat");
    _requestWaiterUri = String("/api/v1/request/waiter");
    _requestBillUri = String("/api/v1/request/bill");

    _barPadId = barPadId;

    _heartbeatJson = "{\"tableid\":\"";
    _heartbeatJson.concat(barPadId);
    _heartbeatJson.concat("\"}");

    _client.setReuse(true);

    // Space to insert 2 UID - table (BarPad) and request
    _requestBuffer = new char[strlen(_requestFmt) + 2 * 32 + 1];
}

bool Communicator::Heartbeat(const char *tableId)
{
    DebugRequestUri("POSTing Heartbeat to: ", _host.c_str(), _heartbeatUri.c_str(), _port);

    _client.begin(_host, _port, _heartbeatUri);
    bool isConnected = IsHttpClientConnected(&_client, TimeoutMs);
    int responseCode = _client.POST(_heartbeatJson);
    Serial.print("Response code of heartbeat: ");
    Serial.println(responseCode);
    if (responseCode > 199 && responseCode < 300)
    {
        String response = _client.getString();
        Serial.println(response);
    }

    _client.end();

    return responseCode == 200;
}

bool Communicator::RequestWaiter(const char *tableId, const char *requestId)
{
    DebugRequestUri("POSTing request for waiter to: ", _host.c_str(), _requestWaiterUri.c_str(), _port);
    sprintf(_requestBuffer, _requestFmt, _barPadId.c_str(), requestId);
    Serial.println(_requestBuffer);

    _client.begin(_host, _port, _requestWaiterUri);
    IsHttpClientConnected(&_client, TimeoutMs);
    int responseCode = _client.POST(_requestBuffer);
    Serial.print("Response code of request for waiter: ");
    Serial.println(responseCode);
    if (responseCode > 199 && responseCode < 300)
    {
        String response = _client.getString();
        Serial.println(response);
    }

    _client.end();

    return responseCode == 200;
}

bool Communicator::RequestBill(const char *tableId, const char *requestId)
{
    DebugRequestUri("POSTing request for bill to: ", _host.c_str(), _requestBillUri.c_str(), _port);
    sprintf(_requestBuffer, _requestFmt, _barPadId.c_str(), requestId);
    Serial.println(_requestBuffer);

    _client.begin(_host, _port, _requestBillUri);
    IsHttpClientConnected(&_client, TimeoutMs);
    int responseCode = _client.POST(_requestBuffer);
    Serial.print("Response code of request for bill: ");
    Serial.println(responseCode);
    if (responseCode > 199 && responseCode < 300)
    {
        String response = _client.getString();
        Serial.println(response);
    }

    _client.end();

    return responseCode == 200;
}

bool Communicator::CancelAllRequests(const char *tableId)
{
    Serial.println("Stub for cancel all requests...");
    return false;
}

bool Communicator::IsHttpClientConnected(HTTPClient *client, ulong timeoutMs)
{
    ulong now = millis();
    while (client->connected() == 0 && millis() - now < timeoutMs)
    {
        Serial.print(".");
        delay(50);
    }
    Serial.println("");

    return client->connected();
}

void Communicator::DebugRequestUri(const char *msg, const char *host, const char *uri, int port)
{
    Serial.print(msg);
    Serial.print(host);
    Serial.print(":");
    Serial.print(port);
    Serial.println(uri);
}

Communicator::~Communicator()
{
    if (_requestBuffer != nullptr)
    {
        delete[] _requestBuffer;
    }
}

#endif
