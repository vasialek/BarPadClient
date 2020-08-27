#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#ifdef ESP32
    #include <HTTPClient.h>
#else
    #include <ESP8266HTTPClient.h>
#endif

class Communicator
{
private:
    HTTPClient _client;
    String _baseUrl;
    String _barPadId;
    String _heartbeatJson;
public:
    Communicator(const char* baseUrl, const char* barPadId);
    bool Heartbeat();
    ~Communicator();
};

Communicator::Communicator(const char* baseUrl, const char* barPadId)
{
    _baseUrl = baseUrl;
    _barPadId = barPadId;

    _heartbeatJson = "{\"tableid\":\"";
    _heartbeatJson.concat(barPadId);
    _heartbeatJson.concat("\"}");
    
    Serial.print("Heartbeat JSON: ");
    Serial.println(_heartbeatJson);
}

bool Communicator::Heartbeat()
{
    String url = _baseUrl + "post";
    Serial.print("POSTing Heartbeat to: ");
    Serial.println(url);
    _client.begin(url.c_str());
    _client.setTimeout(10000);

    int responseCode = _client.POST(_heartbeatJson);
    Serial.print("Response code of heartbeat: ");
    Serial.println(responseCode);

    _client.end();
    
    return responseCode == 200;
}

Communicator::~Communicator()
{
}


#endif
