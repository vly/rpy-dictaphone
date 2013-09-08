# pir-monitor

# Hardware setup

The PIR-sensor we are using has 3 wires (red, brown, black). With the Pi ethernet jack facing towards you, and the SD slot facing away from you, plug the red wire into the 5V pin at the top right. Plug the brown wire into the GRD pin, which is third from the top right. Plug the black wire into GPIO pin 7, which is 4th on the left.

| Left  | Right  |
| :---: | :----: |
| 0     | RED    |
| 0     | 0      |
| 0     | BROWN  |
| BLACK | 0      |
| 0     | 0      |
| 0     | 0      |
| 0     | 0      |
| 0     | 0      |
| 0     | 0      |
| 0     | 0      |
| 0     | 0      |
| 0     | 0      |
| 0     | 0      |



# Software setup

This version of the program is using the [bcm2835](http://www.airspayce.com/mikem/bcm2835/) C library. First of all, download the library and install it.

```
cd bcm2835
./configure
make
sudo make check
sudo make install
```
Finally, build and run the PIR monitor program:

```
make
sudo ./pir-monitor
```