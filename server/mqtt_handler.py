# mqtt_handler.py

import time
import paho.mqtt.client as mqtt
from config import MQTT_BROKER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, MQTT_TOPICS, TRACKED_BLE_MACS
from tracking import process_received_rssi

def on_message(client, userdata, msg):
    """
    Callback pour traiter les messages MQTT reçus.
    """
    topic = msg.topic
    payload = msg.payload.decode("utf-8")

    try:
        parts = payload.split(",")
        if len(parts) < 3:
            print(f"Message mal formé sur {topic}: {payload}")
            return
        
        ble_id = parts[1].strip()
        if ble_id not in TRACKED_BLE_MACS:
            return

        rssi = int(parts[2].replace("rayon", "").strip())
        process_received_rssi(ble_id, topic, rssi)

    except Exception as e:
        print(f"Erreur lors du parsing du message '{payload}': {e}")

def on_connect(client, userdata, flags, reason_code, properties):
    """
    Callback pour gérer la connexion au broker MQTT.
    """
    if reason_code == 0:
        print("Connexion réussie au broker MQTT")
        for topic in MQTT_TOPICS:
            client.subscribe(topic)
    else:
        print("Échec de la connexion, code =", reason_code)

def setup_mqtt():
    """
    Initialise le client MQTT et démarre la connexion.
    """
    client = mqtt.Client()
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    
    client.on_connect = on_connect
    client.on_message = on_message
    
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    
    return client
