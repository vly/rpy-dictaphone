import RPi.GPIO as GPIO
import time
import curses

stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.refresh()

GPIO.setmode(GPIO.BOARD)
GPIO.setup(7,GPIO.OUT)

p = GPIO.PWM(7,50)
key = ''

try:
        current = 4.5
        print("init...")
        p.start(current)
        while True:
                key = stdscr.getch()
                stdscr.addch(0,0,key)
                stdscr.refresh()
                if key == ord('6'):
                       if current < 15.3:
                                current += 0.2
                                p.ChangeDutyCycle(current)
                                time.sleep(0.001)
                if key == ord('4'):
                        if current > 4.4:
                                current -= 0.2
                                p.ChangeDutyCycle(current)
                                time.sleep(0.001)
                key = ''
except KeyboardInterrupt:
        GPIO.cleanup()

finally:
        GPIO.cleanup()
        curses.endwin()
