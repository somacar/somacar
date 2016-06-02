import bluetooth
import subprocess
import serial
import signal
from subprocess import call, PIPE, STDOUT
import subprocess
import time
import shlex
import os

BT_ap_mode = False

Serial_Location_port = "/dev/ttyACM0"

def runProcess(p): #execute System command
        yield "pid=" + str(os.getpgid(p.pid))
        while(True):
                retcode = p.poll() #returns None while subprocess is running
                line = p.stdout.readline()
                yield line
                if(retcode is not None):
                        networkProc_pid = ""
                        break

def SerialReceiver():
	p_sr = serial.Serial(port_location,9600)
	p_sr.flushInput()
	while True:
    try:
	    debug = str(p_sr.readline())
	    test = debug.replace(r"\r\n" ,"")
	    test3 = test.replace("b'", "")
	    SerialString = test3.replace("'" , "")
	    # must be APName/APPass    
	    print(SerialString)


def BluetoothReceiver():
	# Init bluetooth for receive
	p = subprocess.Popen("sudo hciconfig hci0 piscan", stdout=subprocess.PIPE, stderr=subprocess.STDOUT,shell=True)
	server_sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	port = 1
	server_sock.bind(("",port))
	server_sock.listen(1)
	print("listening bluetooth signal from Client...")

	try:
		client_sock, address = server_sock.accept()
		print("Accepted connection from ", address)
		while 1:
			data = client_sock.recv(1024)
			command_str = str(data).split("'")[1]
			client_sock.send(data)

			bluetoothHandler(command_str, client_sock)

	except:
		print("Error", e)
		client_sock.close()
		server_sock.close()

def bluetoothHandler(value, client_sock):
	
	if value == "wifi":
		print("Wi-Fi Mode Enabled")
		BT_ap_mode = True

	if BT_ap_mode == True and not(value == "wifi"):
		process = value.split("/")
		print("AP Set process..")
    	print("---------------------------------")
    	result = call(["wpa_passphrase", process[0], process[1]])
    	print("[ROOT] Saving configure file")
    	call("wpa_passphrase " + process[0] + " " + process[1] + "> " + wpa_configure_file ,shell=True)


