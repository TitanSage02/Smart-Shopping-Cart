# tracking.py
import time
from collections import defaultdict
from trilateration import rssi_to_distance, trilaterate
from config import anchors_positions, TIME_THRESHOLD
import threading

measurements = defaultdict(dict)
data_lock = threading.Lock()

latest_estimated_position = None
latest_ble_id = None
position_lock = threading.Lock()

def process_received_rssi(ble_id, anchor, rssi):
    """
    Stocke les mesures reçues et met à jour les distances.
    """
    now = time.time()
    with data_lock:
        measurements[ble_id][anchor] = (now, rssi)

def process_measurements():
    """
    Thread de calcul des positions.
    """
    global latest_estimated_position, latest_ble_id

    while True:
        time.sleep(1)
        now = time.time()

        with data_lock:
            for ble_id in list(measurements.keys()):
                anchors_data = {anchor: (t, rssi) for anchor, (t, rssi) in measurements[ble_id].items() if now - t < TIME_THRESHOLD}
                if len(anchors_data) < 3:
                    continue

                pos_list = []
                dist_list = []
                for anchor, (t, rssi) in anchors_data.items():
                    if anchor in anchors_positions:
                        pos_list.append(anchors_positions[anchor])
                        dist_list.append(rssi_to_distance(rssi))

                if len(pos_list) < 3:
                    continue

                est_pos = trilaterate(pos_list, dist_list)
                with position_lock:
                    latest_estimated_position = est_pos
                    latest_ble_id = ble_id

                del measurements[ble_id]
