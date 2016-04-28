import serial
import time
from subprocess import call

port_location = "/dev/ttyACM0"
wpa_configure_file = "/etc/wpa.config"


p_sr = serial.Serial(port_location,9600)
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
    	print("AP Set process..")
    	print("---------------------------------")
    	result = call(["wpa_passphrase", process[0], process[1]])
    	print("[ROOT] Saving configure file")
    	call("wpa_passphrase " + process[0] + " " + process[1] + "> " + wpa_configure_file ,shell=True)
    	print("[ROOT]network service restarting")
    	call("service networking restart",shell=True)
    	print("[ROOT]Restart Completed")
    	ap_mode = False
