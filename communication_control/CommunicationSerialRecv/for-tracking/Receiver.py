import serial

port_location = "/dev/ttyAMA0"

p_sr = serial.Serial(port_location, 9600)
p_sr.flushInput()

while True:
        debug = str(p_sr.read(1))

        _debug = debug.split("'")[1]

        if _debug == "8":
                print("Attached!")

        if _debug == "9":
                print("Attached (Terminate)")
