void setup() 
{
  pinMode (LED_BUILTIN, OUTPUT); //Initiation de la led en tant que sortie
}

// Boucle pour la lecture des donnees
void loop() 
{
  // put your main code here, to run repeatedly:
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500); //En ms = 0.5s 
  digitalWrite(LED_BUILTIN, LOW);
  delay(500); //En ms = 0.5s 

}
