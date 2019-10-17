import os
import serial
from PIL import Image

serialport = serial.Serial("/dev/ttyAMA0", 9600, timeout=0.5)

def main_fn():
    while True:    
        command = serialport.read()
        break

    os.system("adb shell screencap -p /sdcard/screencap.png && adb pull /sdcard/screencap.png")

    image = Image.open("screencap.png")
    width, height = image.size

    tail_width = width / 4
    black_tail_number = 0

    for i in range(4):
        if image[i * tail_width + 20][50] == (0, 0, 0):
            black_tail_number = i

    serialport.write(black_tail_number)

while True:
    main_fn()