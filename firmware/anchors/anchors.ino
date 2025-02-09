/**********************************************
 * Code ESP32 : Scan BLE et publication MQTT
 * Synchronisation de l'heure via un serveur HTTP
 **********************************************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <time.h>
#include <sys/time.h>
#include <HTTPClient.h>
 
// ------------------
// Configuration WiFi
// ------------------
const char* ssid     = "ssid-here";
const char* password = "password-here";
 
// ------------------
// Configuration MQTT
// ------------------
const char* mqtt_server = "broker_adress";
const int   mqtt_port   = 1883;
const char* mqtt_user   = "mqtt_user";
const char* mqtt_pass   = "mqtt_pass";
const char* mqtt_topic  = ""; // anchor_x (x = 1, 2, 3, ...)
 
// ------------------
// Configuration serveur de temps
// ------------------
// Construction de l'URL complète (par exemple "192.168.8.1/time")
String serverURL = String(mqtt_server) + "/time";
 
// ------------------
// Variables globales
// ------------------
WiFiClient espClient;
PubSubClient client(espClient);
BLEScan* pBLEScan;
 
// ------------------
// Fonction pour se connecter au WiFi
// ------------------
void setupWiFi() {
    Serial.print("Connexion au WiFi ");
    Serial.print(ssid);
   
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) { 
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnecté !");
        Serial.print("Adresse IP : ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nÉchec de connexion WiFi !");
    }
}
 
// ------------------
// Fonction pour se connecter au broker MQTT
// ------------------
void reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Connexion au broker MQTT...");
        String clientId = mqtt_topic;  // Peut être personnalisé pour être unique
    
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
        Serial.println("connecté !");
        } else {
        Serial.print("Échec, rc=");
        Serial.print(client.state());
        Serial.println(" nouvelle tentative dans 3 secondes");
        delay(3000);
        }
    }
}
 
// ------------------
// Fonction pour synchroniser l'heure via le serveur HTTP
// Cette requête est effectuée une seule fois lors du setup
// ------------------
void setupTime() {
    HTTPClient http;
    http.begin(serverURL.c_str());
    int httpCode = http.GET();
        
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Heure reçue (timestamp Unix) : " + payload);
        
        // Convertir le payload en timestamp Unix
        time_t t = (time_t)payload.toInt();
        if (t > 0) {
            // Mise à jour de l'heure système
            struct timeval tv;
            tv.tv_sec = t;
            tv.tv_usec = 0;
            settimeofday(&tv, NULL);
            Serial.println("Heure système mise à jour !");
        } else {
            Serial.println("Erreur lors de la conversion du timestamp.");
        }
    } else {
        Serial.print("Erreur HTTP : ");
        Serial.println(httpCode);
    }
    http.end();
    
    Serial.println("\nHeure synchronisée");
 }
 
// ------------------
// Fonction pour obtenir l'heure formatée (HH:MM:SS)
// ------------------
String getTimeString() {
    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_r(&now, &timeInfo);
    
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
                timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    return String(buffer);
}
 
// ------------------
// Fonction de publication MQTT
// ------------------
void publishMessage(const String &message) {
    Serial.print("Publication du message: ");
    Serial.println(message);
    
    if (client.publish(mqtt_topic, message.c_str())) {
        Serial.println("Message publié avec succès");
    } else { 
        Serial.println("Échec de la publication");
    }
}
 
// ------------------
// Fonction d'initialisation du BLE
// ------------------
void setupBLE() {
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
}
 
// ------------------
// Fonction pour gérer le scan BLE
// ------------------
void handleBLEScan() {
    Serial.println("Démarrage du scan BLE...");
    BLEScanResults* foundDevices = pBLEScan->start(2, false);
    
    int count = foundDevices.getCount();
    Serial.print("Nombre d'appareils trouvés : ");
    Serial.println(count);
    
    for (int i = 0; i < count; i++) {
        BLEAdvertisedDevice device = foundDevices.getDevice(i);
        String addr = device.getAddress().toString();
        String currentTime = getTimeString();
        int rssi = device.getRSSI();
        
        String message = currentTime + ", " + addr + ", RSSI: " + String(rssi);
        
        Serial.print("Appareil trouvé : ");
        Serial.println(message);
    
        publishMessage(message);
    }
 
    pBLEScan->clearResults(); // Libère la mémoire
 }
 
// ------------------
// Setup
// ------------------
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    setupWiFi();
    setupTime();    // Réalise la requête HTTP pour se mettre à l'heure
    
    client.setServer(mqtt_server, mqtt_port);
    setupBLE();
 }
 
// ------------------
// Loop
// ------------------
void loop() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    
    client.loop();  
    
    handleBLEScan();
    
    delay(2000);
}