#include <Arduino.h>

void setup() {
  Serial.begin(9600);  // Le débit doit être le même que celui configuré côté Python
  Serial.println("Initialisation terminée.");
}

void loop() {
  if (Serial.available() > 0) {
    // Lire une ligne complète jusqu'au caractère '\n'
    String input = Serial.readStringUntil('\n');
    Serial.print("Message reçu : ");
    Serial.println(input);

    // Variables pour stocker les valeurs extraites
    float newDirection = 0.0;
    float turningAngle = 0.0;

    // Convertir la chaîne Arduino en tableau de caractères
    char buf[64];
    input.toCharArray(buf, 64);

    // Utilisation de sscanf pour parser la chaîne
    int parsed = sscanf(buf, "DIRECTION: %f deg, VIRAGE: %f deg", &newDirection, &turningAngle);

    if (parsed == 2) {
      Serial.print("Nouvelle direction : ");
      Serial.print(newDirection);
      Serial.print(" deg, Virage : ");
      Serial.print(turningAngle);
      Serial.println(" deg");

      if (turningAngle > 5.0) {
        Serial.println("Action : tourner à droite");
        // Code ici pour activer le moteur droit/gauche

      } else if (turningAngle < -5.0) {
        Serial.println("Action : tourner à gauche");
        //  Code ici le code pour activer le moteur gauche/droit

      } else {
        Serial.println("Action : continuer tout droit");
        // Code ici le code pour continuer en ligne droite.

      }
    } else {
        
      Serial.println("Erreur de parsing du message.");

    }
    
  }
}

