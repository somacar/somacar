import smbus
import time

bus = smbus.SMBus(1)

address = 0x04

def writeNumber(value):
        bus.write_byte(address, value)
        return -1


def readNumber():
	try:
		number = bus.read_byte(address)
		return number
	except IOError:
		print("IOError")

while True:
        #var = input("Enter 1-9 : ")
        #if not var:
        #        continue

        #writeNumber(var)
        #print("RPI : Hi adrduino , i sent you ", var)
        time.sleep(1)

        number = readNumber()
	
        print("Arduini : Hey RPI, I Received a dight" , number)
	if not number == 0:
		time.sleep(1)
		print(number)
