#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> //Requetes HTTP

#define SERVER_IP "192.168.171.235:1880" //Adresse IP du serveur Node-RED

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
  Serial.print("Connecte! Adresse IP: ");
  Serial.println(WiFi.localIP());
  pinMode (LED_BUILTIN, OUTPUT); //Initiation de la led en tant que sortie
}

void loop() {
  String payload; //ON/OFF
  //Attente de la connexion Wi-Fi
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] debut de la requete GET...\n"); 
    //Configuration de l URL du serveur
    http.begin(client, "http://" SERVER_IP "/Led");  //URL pour la requete GET

    //Envoi de la requete GET
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      //Si la reponse du serveur est OK (HTTP 200)
      if (httpCode == HTTP_CODE_OK) {
        payload = http.getString();  //Recuperation reponse du serveur
        Serial.println("Reponse du serveur : ");
        Serial.println(payload);
      }
    } else {
      //Protection Information
      Serial.printf("[HTTP] GET... echec, erreur : %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    //LED clignote si ON et Est ALLUMEE en permanance sinon.
    if (  payload == "ON") 
    {
    Serial.println("La reponse est bonne");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500); //En ms = 0.5s 
    digitalWrite(LED_BUILTIN, LOW);
    }
    else //condition = OFF mais par default on peut l etteindre.
    {
    Serial.println("La reponse est bonne");
    digitalWrite(LED_BUILTIN, LOW);
    delay(500); //Pour uniformiser les delais
    }

  delay(500); //Attente avant la prochaine requete GET (5 ms)
  }
}
