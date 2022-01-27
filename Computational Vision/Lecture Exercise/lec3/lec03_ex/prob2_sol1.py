import cv2
import numpy as np
from matplotlib import pyplot as plt
import matplotlib

import os
import random
import sys

"""
Handle the command-line arguments
"""
if len(sys.argv) != 2:
    print("Usage: %s in_file" % sys.argv[0])
    sys.exit()

