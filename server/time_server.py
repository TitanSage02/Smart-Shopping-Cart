from flask import Flask, jsonify
from datetime import datetime

app = Flask(__name__)

@app.route('/time', methods=['GET'])
def get_time():
    now = datetime.now()
    # Renvoie le timestamp Unix sous forme d'entier
    return jsonify(time=int(now.timestamp()))

if __name__ == '__main__':
    # Le serveur écoute sur toutes les interfaces réseau, port 5000
    app.run(host='0.0.0.0', port=5000)
