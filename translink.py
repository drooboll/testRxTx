import network
import socket
from machine import UART
from machine import Pin
import gc

uart = UART(0, 115200)
pin = Pin(0, Pin.OUT)
rx_ready = Pin(2, Pin.IN)

ap = network.WLAN(network.AP_IF)
ap.active(True)
ap.config(essid="VSB", password="12121212")

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
addr = socket.getaddrinfo('0.0.0.0', 9998)[0][-1]
sock.bind(addr)
sock.listen(1)

def run():
	while True:
		pin.value(1)
		client_sock, ip = sock.accept()
		#print(ip)
		pin.value(0)
		while True:
			data = client_sock.recv(12)
			if len(data):
				if rx_ready.value():
					uart.write(data)
					gc.collect()
			else:
				client_sock.close()
				break

		
