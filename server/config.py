# config.py

# MQTT Configuration
MQTT_BROKER = "broker_adress"
MQTT_PORT = 1883
MQTT_USERNAME = "mqtt_user"
MQTT_PASSWORD = "mqtt_password"
MQTT_TOPICS = ["anchor_1", "anchor_2", "anchor_3"]

# RSSI → Distance Model
RSSI_REF = -50
PATH_LOSS_EXPONENT = 2.0

# Seuil de validité des mesures
TIME_THRESHOLD = 5

# Port série
SERIAL_PORT = "COM4"
BAUD_RATE = 9600

# Positions des ancres (en mètres)
anchors_positions = {
    "esp1": (0.0, 0.0),
    "esp2": (5.0, 0.0),
    "esp3": (0.0, 3.0),
}

# Appareils BLE à suivre
TRACKED_BLE_MACS = {
    "08:A6:F7:B1:7C:AE",
}
