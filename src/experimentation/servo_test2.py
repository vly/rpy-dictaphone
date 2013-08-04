'''Testing servoblaster daemon operations'''

import sys
import tty
import termios

# constants
MAX_LEFT = 90
MAX_RIGHT = 200
STARTING_POINT = 150
INPUTS = {'a' : -1,
          'd' : 1}

# move servo
def shift(servo, position):
    open('/dev/servoblaster', 'w').write("%d=%d\n" % (servo, position))

# get single char input
def get_input():
    current_pos = STARTING_POINT
    while True:
        char = sys.stdin.read(1)
        if char is'q':
            print('Exiting.')
            break
        if char is 'r':
            current_pos = STARTING_POINT
            shift(0, current_pos)
        if char in INPUTS:
            if current_pos + INPUTS[char] <= MAX_RIGHT and \
               current_pos + INPUTS[char] >= MAX_LEFT:
                current_pos += INPUTS[char]
                print('Input: %d\r' % current_pos)
                shift(0, current_pos)

# run test
def manual_test():
    fd = sys.stdin.fileno()
    old = termios.tcgetattr(fd)
    try:
        print('Use "a" and "d" to position the servo, "r" to reset, and "q" to exit.')
        tty.setraw(fd)
        get_input()
    except KeyboardInterrupt:
        print('Exiting.')
    finally:
        termios.tcsetattr(fd, termios.TCSAFLUSH, old)

# init
if __name__ == '__main__':
    manual_test()
