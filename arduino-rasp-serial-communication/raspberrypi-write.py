import serial
import time

port = "/dev/ttyACM4"
p_sr = serial.Serial(port, 9600)
p_sr.flushInput()

default_string = "torap"

while True:
	print("test")
	p_sr.write(bytes("1", 'UTF-8'))	
	time.sleep(100 / 1000.0)	
	debug = p_sr.readline()
	print(debug)
