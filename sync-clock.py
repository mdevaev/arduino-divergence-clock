#!/usr/bin/env python


import sys
import time

import serial


# =====
def main():
    tty = serial.Serial(sys.argv[1], 115200)
    now = time.localtime()
    cmd = "%.2d %.2d %.2d" % (now.tm_hour, now.tm_min, now.tm_sec)
    print(cmd)
    tty.write(cmd.encode())


if __name__ == "__main__":
    main()
