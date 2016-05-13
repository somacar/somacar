import serial
import time
import shlex
import os
from subprocess import call, PIPE, STDOUT
import signal

import socket
REMOTE_SERVER = "www.google.com"
ap_mode = False
port_location = "/dev/ttyACM0"
wpa_configure_file = "/etc/wpa.config"
p_sr = serial.Serial(port_location,9600)
p_sr.flushInput()

def signal_handler(signum, frame):
    raise Exception("Timed out!")



def get_return_code_of_simple_cmd(cmd, stderr=STDOUT):
    """Execute a simple external command and return its exit status."""
    args = shlex.split(cmd)
    return call(args, stdout=PIPE, stderr=stderr)

def is_network_alive(gateway_ip):
    cmd = "ping -c 1 " + gateway_ip
    return get_return_code_of_simple_cmd(cmd) == 0

def timeout( p ):
    if p.poll() is None:
        print ("Error: process taking too long to complete--terminating")
        p.kill()

def runProcess(exe): #execute System command
	p = subprocess.Popen(exe, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	while(True):
		retcode = p.poll() #returns None while subprocess is running
		line = p.stdout.readline()
		yield line
		if(retcode is not None):
			break


while True:
    try:
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
	    	# signal.signal(signal.SIGALRM, signal_handler)
	    	# signal.alarm(20)   # Ten seconds limit for Network restart
	    	try:
	    	    	call("service networking stop",shell=True)
	    	except Exception:
			print("[ROOT] command error")
			count = 0
			ip_set = False
			gateway_ip = ""


		try:
			for line in runProcess("service networking restart")
				if line.find("DHCPDISCOVER"):
					count = count + 1
					if count == 5:
						print("[ROOT] Cannot connect to AP")
						break

					if line.find("DHCPACK"):
						split_string = line.split("from ")
						gateway_ip = split_string[1]
						ip_set = True

		except:
			print("[ROOT] Unexcepted error occured")


	    	if (is_network_alive(gateway_ip)):
	    	    	p_sr.write(bytes("1", 'UTF-8'))
	    	    	print("[ROOT] Wi-Fi Connected")
	    	
	    	if (gateway_ip == "")
	    	    	p_sr.write(bytes("2", 'UTF-8'))
	    	    	print("[ROOT] Failed to connect.")

	    	ap_mode = False

    except serial.SerialException as e:
        print('OS Error')
    except TypeError as e:
        print('Type Error')
=======
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
<<<<<<< HEAD:arduino-raspberry-wifi-connect/wifi-serial.py
    	p_sr.write(bytes("1", 'UTF-8'))
    	ap_mode = False
=======
	p_sr.write(bytes("1", 'UTF-8'))
	ap_mode = False
	
>>>>>>> origin/rainc:arduino-raspberry-wifi-configuration/wifi-serial.py
>>>>>>> origin/gunhee
