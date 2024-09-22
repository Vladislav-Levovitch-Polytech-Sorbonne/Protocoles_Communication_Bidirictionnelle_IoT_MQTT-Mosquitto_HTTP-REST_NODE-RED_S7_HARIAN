#include <DHT.h> //Librairie pour le capteur 
#include <ESP8266WiFi.h>



#define DHTPIN 13  //GPIO13 = broche D7
#define DHTTYPE DHT11

//Initialisation du capteur
DHT dht(DHTPIN, DHTTYPE); 

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Communication avec DTH11 demarree");
  dht.begin();
  Serial.println("Debut des relevees capteur DTH11 avec un intervalle de mesure de 0,5 s");
}

// Boucle pour la lecture des donnees
void loop() 
{
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

  // Test des donnees
  Serial.println("Temperature : " + String(temperature)+" °C");
  Serial.println("Humidite : " + String(humidite)+" %");
}
