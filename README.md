# Smart-Shopping Cart : Indoor Positioning & Autonomous Shopping Cart

## Overview
Smart-Shopping Cart est un projet de caddie intelligent qui suit automatiquement le client dans une boutique grâce à un système de localisation en intérieur basé sur la trilatération Bluetooth Low Energy (BLE).

Chaque client reçoit un **tag BLE** (ESP32) au début de son parcours. Des **ESP32 ancres** positionnées dans le magasin captent les signaux de ce tag et envoient les RSSI (Received Signal Strength Indicator) à un serveur via MQTT. Le serveur analyse ces données et envoie les commandes de navigation au caddie du client pour qu’il le suive en temps réel.

---
## 📌 Features

✅ **Localisation indoor avec BLE** : Calcul de la position du client par trilatération grâce à plusieurs ESP32 ancrés dans la boutique.
✅ **Suivi autonome du caddie** : Communication entre le serveur et le caddie via MQTT pour un suivi fluide.
✅ **Affichage en temps réel** : Un écran sur le caddie affiche les produits scannés, leurs prix et d'autres informations.
✅ **Architecture distribuée** : ESP32 pour la collecte des données, un serveur Python pour le traitement et un Arduino pour le contrôle du moteur.
✅ **Communication MQTT efficace** : Transmission rapide et asynchrone des commandes et mesures RSSI.


## 🛠 Architecture Technique
### 1️⃣ **Tag BLE (ESP32)**
- Se connecte au réseau Bluetooth et envoie des signaux BLE.
- Fonctionne sur batterie et est attribué à chaque client.

### 2️⃣ **Ancres ESP32 (BLE Scanners)**
- Disposées dans la boutique pour scanner les signaux BLE.
- Envoient les RSSI et l’adresse MAC détectés au serveur via MQTT.

### 3️⃣ **Serveur de traitement (Python)**
- Analyse les RSSI et effectue une **trilatération** pour estimer la position du client.
- Envoie les commandes au caddie pour le suivi.

### 4️⃣ **Caddie autonome (ESP32 + Arduino + L298N)**
- Reçoit les commandes du serveur et ajuste son mouvement.
- Affichage écran pour montrer le prix des articles scannés.

---
## 📂 Fichiers et Organisation

📂 SmartCart-System/
│── 📂 firmware/             # Code pour ESP32 (tag, caddie et anchors)
│    ├── 📂 tag/             # Firmware des tags (BLE)
│    ├── 📂 anchors/         # Firmware des ESP32 ancres (BLE Scan + MQTT)
│    ├── 📂 cart/            # Firmware du caddie (réception commandes + moteur)
│    ├── 📂 arduino_cart/    # Code pour l’Arduino du caddie (moteurs)
│── 📂 server/               # Code du serveur de traitement
│── 📂 docs/                 # Documentation et images
│── 📂 hardware/             # Schémas et modèles 3D (caddie, électronique)
│── 📂 frontend/             # Interface utilisateur (écran du caddie)
│── README.md                # Présentation globale du projet
│── LICENSE                  # Licence du projet

---
## 🚀 Installation & Déploiement

### 1️⃣ **Installation des dépendances** (Serveur Python)
```sh
cd server
python -m venv venv
source venv/bin/activate  # (ou venv\Scripts\activate sous Windows)
pip install -r requirements.txt
```

### 2️⃣ **Déploiement du Broker MQTT**
Installer Mosquitto en local ou via Docker :
```sh
docker run -d -p 1883:1883 -p 9001:9001 eclipse-mosquitto
```

### 3️⃣ **Flash des ESP32 (Tags, Anchors, Caddie)**
Compiler et flasher chaque ESP32 avec son firmware respectif (voir dossier `firmware/`).

### 4️⃣ **Lancement du serveur**
```sh
python server/server.py & python server/time_server.py 
```

---
## 📌 Améliorations Futures
- 🔹 Améliorer la précision avec des algorithmes de filtrage avancés.
- 🔹 Ajouter un module d’évitement d’obstacles au caddie.
- 🔹 Intégration avec un système de paiement automatique.

---
## 📜 Licence
Ce projet est sous licence MIT - voir le fichier LICENSE pour plus de détails.

---
## 💡 Crédits
Développé par Espérance AYIWAHOUN et contributeurs. 🚀