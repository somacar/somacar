import serial
import time

port = "/dev/ttyACM0"
p_sr = serial.Serial(port,9600)
p_sr.flushInput()

ap_mode = False

while True:
    debug = str(p_sr.readline())
    test = debug.replace(r"\r\n" ,"")
    test3 = test.replace("b'", "")
    test4 = test3.replace("'" , "")
    # must be APName/APPass    
    

    process = test4.split("/")

    if process[0] == "AP_SEND":
    	ap_mode = True

    if (ap_mode == True and not(process[0] == "AP_SEND")):
    	print("AP_NAME", process[0])
    	print("AP_pass", process[1])
    	ap_mode = False
