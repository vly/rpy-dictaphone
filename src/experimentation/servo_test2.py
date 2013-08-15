'''Testing servoblaster daemon operations'''

import sys
import tty
import termios

# constants
X_MAX_LEFT = 164
X_MAX_RIGHT = 249
X_STARTING_POINT = 208

Y_MAX_LEFT = 72
Y_MAX_RIGHT = 249
Y_STARTING_POINT = 198

INPUTS = {'a' : -1,
          'd' : 1,
          'w' : -1,
          's' : 1}

# move servo
def shift(servo_x, servo_y, position_x, position_y):
    open('/dev/servoblaster', 'w').write("%d=%d\n%d=%d\n" % (servo_x,
position_x, servo_y, position_y))

# get single char input
def get_input():
    current_pos_y = Y_STARTING_POINT
    current_pos_x = X_STARTING_POINT
    while True:
        char = sys.stdin.read(1)
        if char is'q':
            print('Exiting.')
            break
        if char is 'r':
            current_pos_x = X_STARTING_POINT
            current_pos_y = Y_STARTING_POINT
            shift(1, 0, current_pos_x, current_pos_y)
        if char is 'z':
            current_pos_x = X_MAX_LEFT
            current_pos_y = Y_MAX_LEFT
            shift(1, 0, current_pos_x, current_pos_y)
        if char is 'x':
            current_pos_x = X_MAX_RIGHT
            current_pos_y = Y_MAX_RIGHT
            shift(1, 0, current_pos_x, current_pos_y)
        if char in INPUTS:
            if char == 'w' or char == 's':
                if current_pos_y + INPUTS[char] <= Y_MAX_RIGHT and \
                   current_pos_y + INPUTS[char] >= Y_MAX_LEFT:
                    current_pos_y += INPUTS[char]
                    print('Input: %d\r' % current_pos_y)
                    shift(1, 0, current_pos_x, current_pos_y)
            else:
                if current_pos_x + INPUTS[char] <= X_MAX_RIGHT and \
                   current_pos_x + INPUTS[char] >= X_MAX_LEFT:
                    current_pos_x += INPUTS[char]
                    print('Input: %d\r' % current_pos_x)
                    shift(1, 0, current_pos_x, current_pos_y)

# run test
def manual_test():
    fd = sys.stdin.fileno()
    old = termios.tcgetattr(fd)
    try:
        print('Use "a", "d", "w" and "s" to position the servos, "r" to reset,\
"z" for max left, "x" for max right, and "q" to exit.')
        tty.setraw(fd)
        get_input()
    except KeyboardInterrupt:
        print('Exiting.')
    finally:
        termios.tcsetattr(fd, termios.TCSAFLUSH, old)

# init
if __name__ == '__main__':
    manual_test()
