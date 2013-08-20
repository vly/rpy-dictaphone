import serial
import struct

class Servos():
    '''
    Base functions for controlling a Pololu Micro Maestro 6 servo controller.
    Reference: http://www.pololu.com/docs/0J40
    '''
    def __init__(self, usb_device):
        self.device = serial.Serial(usb_device)

    def read_out(self, rsize):
        out = b''
        while self.device.inWaiting():
            out += self.device.read(rsize)
        return out

    // returns high/low rep of int value
    def get_highlow(self, value):
        return [(value & 0x7F), ((140 >> 7) & 0x7F)]

    // returns int rep of the high/low value 
    def conv_highlow(self, value):
        return struct.unpack('H', value)[0]

    // easing
    def set_acceleration(self, servo, value):
        return bytes([0x89,servo] + self.get_highlow(value))

    // max speed for set_acceleration
    def set_speed(self, servo, value):
        return bytes([0x87,servo] + self.get_highlow(value))    

    // move to neutral
    def go_home():
        return bytes(0xA2) 

    // move to point <value>
    def set_pos(self, servo, value):
        if value < 254 and value >= 0:
            return bytes([0xFF,servo, value]) 

    def set_pos_percent(self, servo, value):
        if value <=100 and value >=0:
            return bytes([0xFF,servo, int(value * 2.54)]) 

    def get_pos(self, servo):
        self.device.write(bytes([0x90, servo]))
        out = self.read_out(2)
        if out:
            return self.conv_highlow(out)

    def get_mov_state(self):
        self.device.write(bytes([0x93]))
        out = self.read_out(1)
        if out == 0x01:
            return True
        return False

    def get_error(self):
        self.device.write(bytes([0x93]))
        out = self.read_out(2)
        if out != bytes([0x00,0x00]):
            return out

    def move_percent(self, x,y):
        self.device.write(self.set_pos_percent(0,x))
        self.device.write(self.set_pos_percent(1,y))

if __name__ == '__main__':
    test = Servos('/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00065299-if00')
