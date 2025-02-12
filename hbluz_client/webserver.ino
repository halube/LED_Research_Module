#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


bool passwordProtected = false;

ESP8266WebServer server(80);

void setupWebserver()
{
    if (MDNS.begin(deviceName.c_str())) {
#ifdef DEBUG
        Serial.println("MDNS responder started");
#endif
    }

    server.on("/getProgram", []() {
        server.send(200, "application/json", getProgramFromMemory());
    });

    server.on("/getStatus", []() {
        server.send(200, "application/json", getStatus());
    });

    server.on("/getTemperature", []() {
        server.send(200, "application/json", getTemperatureAsJSON());
    });

    server.on("/postProgram", []() {
        putProgramIntoMemory(server.arg("plain"));
        resetLEDS();
        parseJSON(server.arg("plain"));
        setRTCTime(SyncDateTime);
        setPulseFrequency(pulseFrequency);      
        server.send(200, "text/plain", "Digga habs!");
    });

    server.onNotFound(handleNotFound);

    server.begin();

#ifdef DEBUG
    Serial.println("HTTP server started");
#endif // DEBUG
}

void loopWebserver()
{
    server.handleClient();
}

void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

String getStatus()
{
    String temp = "{\"DeviceName\":";
    temp += "\"" + deviceName + "\",";
    temp += "\"ProgramName\":";
    temp += "\"" + programName + "\",";
    temp += "\"PasswordProtected\":";
    temp += "\"" + (currentStatus == ClientStatus::RUNNING ? String(passwordProtected) : String(0)) + "\",";
    temp += "\"Status\":";

    switch (currentStatus) {
    case ClientStatus::READY:
        temp += "0}";
        break;
    case ClientStatus::RUNNING:
        temp += "1}";
        break;
    case ClientStatus::FINISHED:
        temp += "2}";
        break;
    }

    return temp;
}
