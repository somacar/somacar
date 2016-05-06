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

def is_network_alive():
    cmd = "ping -c 1 www.google.com"
    return get_return_code_of_simple_cmd(cmd) == 0

def timeout( p ):
    if p.poll() is None:
        print ("Error: process taking too long to complete--terminating")
        p.kill()


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
	    	signal.signal(signal.SIGALRM, signal_handler)
	    	signal.alarm(20)   # Ten seconds limit for Network restart
	    	try:
	    	    	call("service networking restart",shell=True)
	    	except Exception:
	    	    	print("[ROOT] Network Timeout ")
		
	    	print("[ROOT]Restart Completed")
	    	signal.alarm(0)
	    	if (is_network_alive() == True):
	    	    	p_sr.write(bytes("1", 'UTF-8'))
	    	    	print("[ROOT] Wi-Fi Connected")
	    	else:
	    	    	p_sr.write(bytes("2", 'UTF-8'))
	    	    	print("[ROOT] Failed to connect.")
	
	    	ap_mode = False
    except serial.SerialException as e:
        print('OS Error')
    except TypeError as e:
        print('Type Error')
