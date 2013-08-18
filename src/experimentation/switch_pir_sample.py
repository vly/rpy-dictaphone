import RPi.GPIO as GPIO
import time
import threading

class Standby():
    def __init__(self):
        self.PIR_PIN = 11
        self.SWITCH_PIN = 7
        self.pir_thread = False

        GPIO.setmode(GPIO.BOARD)
        #GPIO.setup(self.PIR_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(self.SWITCH_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

        self.listen_switch()

    def listen_switch(self):
        GPIO.add_event_detect(self.SWITCH_PIN, GPIO.RISING, bouncetime=300)

        while True:
            if GPIO.event_detected(self.SWITCH_PIN):
                if GPIO.input(self.SWITCH_PIN):
                    if not self.pir_thread:
                        self.pir_thread = Listen_movement(self.PIR_PIN)
                        print("Thread started")
                        self.pir_thread.start()
                else:
                    if self.pir_thread:
                        self.pir_thread.stop()
                        print("Thread stopped")
                        self.pir_thread = False

class Listen_movement(threading.Thread):
    def __init__(self, PIR_PIN):
        super(Listen_movement, self).__init__()
        self.PIR_PIN = PIR_PIN
        GPIO.setup(self.PIR_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        self.switch_state = True

    def run(self):
        GPIO.add_event_detect(self.PIR_PIN, GPIO.RISING)

        while self.switch_state:
            if GPIO.event_detected(self.PIR_PIN):
                print("Movement detected")

    def stop(self):
        self.switch_state = False
        print("Thread exited")


if __name__ == "__main__":
    go = Standby()
