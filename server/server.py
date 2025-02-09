# server.py
import threading
from mqtt_handler import setup_mqtt
from tracking import process_measurements
from visualization import live_plot

MAKE_PLOTS = False


if __name__ == "__main__":
    mqtt_client = setup_mqtt()

    threading.Thread(target=process_measurements, daemon=True).start()
    
    if MAKE_PLOTS:
        threading.Thread(target=live_plot, daemon=True).start()

    mqtt_client.loop_forever()
