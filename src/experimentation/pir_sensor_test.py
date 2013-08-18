import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BOARD)
GPIO.setup((7), GPIO.IN, pull_up_down=GPIO.PUD_UP)

GPIO.add_event_detect(7, GPIO.RISING)

while True:
    if GPIO.event_detected(7):
        print("Movement")
