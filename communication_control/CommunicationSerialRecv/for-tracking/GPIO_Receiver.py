#!/usr/bin/env python
from time import sleep           # Allows us to call the sleep function to slow down our loop
import RPi.GPIO as GPIO           # Allows us to call our GPIO pins and names it just GPIO
import subprocess
import threading
import re
import os
 
GPIO.setmode(GPIO.BCM)           # Set's GPIO pins to BCM GPIO numbering
INPUT_PIN = 4           # Sets our input pin, in this example I'm connecting our button to pin 4. Pin 0 is the SDA pin so I avoid using it for sensors/buttons
GPIO.setup(INPUT_PIN, GPIO.IN)           # Set our input pin to be an input

class Processclass(threading.Thread):
	def __init__(self):
		self.stdout = None
		self.stderr = None
		threading.Thread.__init__(self)

	def start(self):
		p = subprocess.Popen('sudo /etc/init.d/somacar start &', shell=True)

	

	def stop(self):
		p = subprocess.Popen('sudo /etc/init.d/somacar stop', shell=True)




# check Opencv daemon status
Program_status = False
processname = 'opencv'
tmp = os.popen("ps -Af").read()
proccount = tmp.count(processname)


if proccount > 0: # If Opencv daemon is running
	print("Already Started")
	Program_status = True # set status to running
	# print(proccount, ' processes running of ', processname, 'type')

while True: 
	print("Check")
	if (GPIO.input(INPUT_PIN) == True): # Physically read the pin now
		if Program_status == False: 
			print("Daemon started")
			loader = Processclass()
			loader.start()
			# subprocess.call("sudo /etc/init.d/somacar start" ,shell=True, stdout=subprocess.PIPE)
			Program_status = True

	else: 
		if Program_status == True: # terminate opencv process
			print("Daemon stopped")
			loader = Processclass()
			loader.stop()
			Program_status = False
	sleep(1)
