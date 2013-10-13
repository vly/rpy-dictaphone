#!/usr/bin/env python

import sys
import os
import time
import getopt
import atexit
import alsaaudio
from daemon import Daemon

def get_fname():
    prefix = "/root/audio/output/recording_"
    return prefix + time.strftime("%d-%m-%y_%H-%M", time.gmtime()) + '.wav'

def shutdown(daemon, pipename):
    daemon.cleanup()
    os.remove(pipename)
    print("Shutting down")
    
class Recording(Daemon):
    def cleanup(self):
        # convert to mp3 and die
        pass

    def run(self):
        card = 'default'
        f = open(get_fname(), 'wb')

        inp = alsaaudio.PCM(alsaaudio.PCM_CAPTURE, alsaaudio.PCM_NONBLOCK, card)

        # Set attributes: Mono, 44100 Hz, 16 bit little endian samples
        inp.setchannels(1)
        inp.setrate(44100)
        inp.setformat(alsaaudio.PCM_FORMAT_S16_LE)

        inp.setperiodsize(160)
        while True:
           # Read data from device
            l, data = inp.read()
          
            if l:
                f.write(data)
                time.sleep(.001)
        f.close()

if __name__ == "__main__":
    named_pipe = '/tmp/osp.fifo'
    command = ''
    daemon = Recording('/tmp/recording_daemon.pid')

    try:
        os.mkfifo(named_pipe)
    except OSError:
        print("Error creating pipe, guess it exists")
        #sys.exit(1)

    while command != 'shutdown':
        pipe = open(named_pipe, 'r')
        command = pipe.read()[:-1]
        print("command received: %s" % command)
        if command == 'start':
            print("starting recording")
            a = os.fork()
            if a == 0:
                daemon.start()
        if command == 'stop':
            print("stopping recording")
            daemon.stop()
        if command == 'shutdown':
            print("stopping recording")
            os.kill(a, 0)
            daemon.stop()
            pipe.close()

    atexit.register(shutdown, daemon, named_pipe)

