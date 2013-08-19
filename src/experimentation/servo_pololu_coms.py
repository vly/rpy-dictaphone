import serial

class Servos():
    '''
    Base functions for controlling a Pololu Micro Maestro 6 servo controller.
    Reference: http://www.pololu.com/docs/0J40
    '''
    def __init__(self, usb_device):
        self.device = serial.Serial(usb_device)

    def self.get_highlow(self, value):
        return [(value & 0x7F), ((140 >> 7) & 0x7F)]

    def self.set_acceleration(self, servo, value):
        return bytes([0x89,servo] + get_highlow(value))

    def self.set_speed(self, servo, value):
        return bytes([0x87,servo] + get_highlow(value))    

    def self.go_home():
        return bytes(0xA2) 

    def self.set_pos(self, servo, value):
        if value < 254 and value >= 0:
            return bytes([0xFF,servo, value]) 

    def self.set_pos_percent(self, servo, value):
        if value <=100 and value >=0:
            return bytes([0xFF,servo, int(value * 2.54)]) 


    def self.move_percent(self, x,y):
        ser.write(set_pos_percent(0,x))
        ser.write(set_pos_percent(1,y))

if __name__ == '__main__':
    test = Servos('/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00065299-if00')
