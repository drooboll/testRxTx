## Test remote control

# How to use

- Put *boot.py* & *translink.py* to ESP8266 (actually unnecessary)
- Load *receiver.ino* to Arduino
- connect Pin 45 on Arduino to IO2 on ESP8266
- Load *joystick.py* to computer
- Connect computer via Wi-Fi to AP *"VSB"* with pass *"12121212"* (4x 12)
- run *joystick.py*

# Bugs and how to avoid them

- Sometimes data is not transferred correctly after reconnect. So:

1. Before disconnecting from AP close python program.
2. If you need to reset ESP8266 - hold reset on Arduino and then press reset on Arduino.
