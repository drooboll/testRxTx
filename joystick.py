# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from pygame import joystick
from pygame import display
from pygame import event
import time
import serial
import struct
import socket

k1 = 222 # Max motor speed
k2 = 200

STOP = 0
DOWN = 1
UP = 2
CLOSE = 1
OPEN = 2

# Choosing port
using_tcp = True

if not using_tcp:
	port = input("Port:")
	dst = serial.Serial('COM{}'.format(port), 9600, timeout=1)
else:
	addr = ('192.168.4.1', 9998)
	dst = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	dst.connect(addr)
	print('Connected to {}'.format(addr))
	

display.init()
joystick.init()
ctrl = joystick.Joystick(0) # Getting connected joystick
ctrl.init()
print(ctrl.get_numaxes())

def g(x, y):
	return (1-abs(x))*y+y

def h(x, y):
	return (1-abs(y))*x+x

last_mode = False
auto_mode = False  

while True:
	event.pump()

	upb = ctrl.get_button(3) # kleshnya up
	dwnb = ctrl.get_button(0)# kleshnya down
	inb = ctrl.get_button(4) # kleshnya in
	outb = ctrl.get_button(5)# kleshnya out
	f_b_b = ctrl.get_axis(2)  #  kleshnya forward-backward
	auto = ctrl.get_button(9)# AUTO 
	if (auto == True and last_mode != auto):
		auto_mode = not auto_mode

	last_mode = auto	
	if upb == True:
		arm_command = UP 
	elif (dwnb == True):
		arm_command = DOWN 
	else:
		arm_command = STOP  

	if inb == True:
		claw_command = UP 
	elif (outb == True):
		claw_command = DOWN 
	else:
		claw_command = STOP  

	#print("MODE = {}".format(auto_mode))
	#print("arm_command = {}".format(arm_command))
	#print("claw_command = {}".format(claw_command))
	if abs(f_b_b) < 1e-4:
		f_b_b = 0
	elif f_b_b > 0:
		f_b_b = 1
	else:
		f_b_b = -1
	if inb == 1:
		kn = 1
	if outb == 1:
		kn = 2

	x = ctrl.get_axis(0)
	y = ctrl.get_axis(1)
	if abs(x) < 1e-4:
		x = 0
	if abs(y) < 1e-4:
		y = 0
	re = (-h(x, y)-g(x, y))*k2/2 #райт энжин
	le = (h(x, y)-g(x, y))*k2/2 #лефт энжин
	
	print("R = {}".format(int(re)))
	print("L = {}".format(int(le)))
	b = bytearray(struct.pack("hhhhh", int(le), int(re), arm_command, claw_command, auto_mode))
	print(b)
	if using_tcp:
		dst.send(b)
	else:
		dst.write(b)
	time.sleep(0.08)
