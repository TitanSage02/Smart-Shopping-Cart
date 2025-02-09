#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define DEVICE_NAME "Tag_BLE"


void setup() {
    Serial.begin(115200);
    Serial.println("Démarrage du périphérique BLE avec adresse MAC personnalisée");
    
    // Initialisation du BLE et création d'un serveur BLE
    BLEDevice::init(DEVICE_NAME);
 
    // Création d'un serveur BLE
    BLEServer *pServer = BLEDevice::createServer();
    
    // Configuration de la publicité BLE
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->setScanResponse(true);  // Active la réponse au scan (optionnel)
    pAdvertising->setMinPreferred(0x06);  // Optimisation pour certains smartphones (iOS notamment)
    pAdvertising->setMinPreferred(0x12);
    
    // Démarrage de la publicité BLE
    BLEDevice::startAdvertising();
    Serial.println("Publicité BLE démarrée");
}
 
void loop() {
    // Le périphérique BLE fonctionne en tâche de fond.
    delay(2000);
}