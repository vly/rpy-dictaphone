import subprocess, time, os, sys
import maestro
cmd = ['./rand_detect']
p = subprocess.Popen(cmd,
                     stdout=subprocess.PIPE,
                     stderr=subprocess.STDOUT)

servos = maestro.Servos('/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00065299-if00')

for line in iter(p.stdout.readline, b''):
    temp = line.rstrip().decode('utf8').split(',')
    servos.move_percent(int(temp[0]), int(temp[1]))
