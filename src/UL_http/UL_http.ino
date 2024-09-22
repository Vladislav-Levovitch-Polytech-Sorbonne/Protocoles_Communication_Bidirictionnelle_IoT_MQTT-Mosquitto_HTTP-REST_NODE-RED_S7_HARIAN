/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> //Envoie de requette HTTP

#define DHTPIN 13  //GPIO13 = broche D7
#define DHTTYPE DHT11

//Initialisation du capteur
DHT dht(DHTPIN, DHTTYPE); 


/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
//#define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER_IP "192.168.171.235:1880"

#ifndef PASSID
#define PASSID "Google"
#define PASSWORD "Vladimir"
#endif

void setup() {

  Serial.begin(115200);
  WiFi.begin(PASSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connection en cours ...");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    // put your main code here, to run repeatedly:
  delay(500); //En ms = 0.5s 

    float humidite = dht.readHumidity();
    float temperature = dht.readTemperature();

    //Test si erreur (important pour les envoies uniques)
    if (isnan(humidite) || isnan(temperature) )
    {
      Serial.print("ERROR ISNOTANUMBER : Verfier liaison capteur et contacter admin");
      return;
    }

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n"); 
    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/donnees/");  // HTTP
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"temperature\": " + String(temperature) + ", \"humidite\": " + String(humidite) + "}";


    Serial.print("[HTTP] POST...\n"); //Debut d envoie POST
    // start connection and send HTTP header and body

    // Envoie de la requete POST avec les donnees JSON et recupere le CODE 200 ou autre si erreur
    int httpCode = http.POST(postData);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(1000);
}
