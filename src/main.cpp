#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "secure.h"
// Here are the WLAN credentials, please uncomment or add them to "secure.h" file
// const char* ssid = "SSID_NAME";
// const char* password = "PASSWORD";

// Values for the OTA access, please uncomment or add them to "secure.h" file
// const int myOTAPort = 8266;
// const char *myOTAPassword = "PASSWORD";
// const char *myOTAHostname = "HOSTNAME";

//Blink LED pin
int ledPin = LED_BUILTIN;

//To make Arduino software autodetect OTA device
WiFiServer TelnetServer(myOTAPort);

void setup()
{
  //To make Arduino software autodetect OTA device
  TelnetServer.begin();

  Serial.begin(115200);
  Serial.println("Booting...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  // Port defaults to 8266
  ArduinoOTA.setPort(myOTAPort);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(myOTAHostname);
  // No authentication by default
  ArduinoOTA.setPassword(myOTAPassword);

  ArduinoOTA.onStart([]() {
    Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA End");
    Serial.println("Rebooting...");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //set LED pin as output
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  ArduinoOTA.handle();

  //Make LED blink
  digitalWrite(ledPin, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  delay(250);
}
