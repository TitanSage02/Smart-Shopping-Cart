import time
import matplotlib.pyplot as plt
from tracking import latest_estimated_position, position_lock
from config import anchors_positions

def live_plot():
    plt.ion()
    fig, ax = plt.subplots()
    ax.set_xlim(-1, 6)
    ax.set_ylim(-1, 6)
    
    for anchor, (x, y) in anchors_positions.items():
        ax.plot(x, y, 'bs', markersize=10)
        ax.text(x + 0.1, y + 0.1, anchor, color='blue')

    ble_point, = ax.plot([], [], 'ro', markersize=10)

    while True:
        with position_lock:
            pos = latest_estimated_position

        if pos is not None:
            ble_point.set_data(pos[0], pos[1])

        fig.canvas.draw()
        fig.canvas.flush_events()
        time.sleep(0.5)
