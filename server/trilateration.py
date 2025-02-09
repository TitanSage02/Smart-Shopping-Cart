# trilateration.py

import math
import numpy as np
from scipy.optimize import least_squares
from config import RSSI_REF, PATH_LOSS_EXPONENT

def rssi_to_distance(rssi):
    """
    Convertit une valeur RSSI en distance (mètres).
    """
    return 10 ** ((RSSI_REF - rssi) / (10 * PATH_LOSS_EXPONENT))

def trilaterate(anchors, distances):
    """
    Calcule la position approximative par trilatération.
    """
    def residuals(p, anchors, distances):
        x, y = p
        return [math.sqrt((x - ax) ** 2 + (y - ay) ** 2) - d for (ax, ay), d in zip(anchors, distances)]
    
    x0 = np.mean([a[0] for a in anchors])
    y0 = np.mean([a[1] for a in anchors])
    result = least_squares(residuals, [x0, y0], args=(anchors, distances))
    
    return result.x
