import serial
import time
import shlex
import os
from subprocess import call, PIPE, STDOUT
import subprocess
import signal


import socket
REMOTE_SERVER = "www.google.com"
ap_mode = False
port_location = "/dev/ttyACM0"
wpa_configure_file = "/etc/wpa.config"
networkProc_pid = ""

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
	# signal.signal(signal.SIGALRM, signal_handler)
	p = subprocess.Popen(exe, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,shell=True,preexec_fn=os.setsid)
	yield "pid=" + os.getpgid(pro.pid)
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

	    	count = 0
	    	ip_set = False
	    	gateway_ip = ""

	    	print("AP Set process..")
	    	print("---------------------------------")
	    	result = call(["wpa_passphrase", process[0], process[1]])
	    	print("[ROOT] Saving configure file")
	    	call("wpa_passphrase " + process[0] + " " + process[1] + "> " + wpa_configure_file ,shell=True)
	    	print("[ROOT]network service restarting")
	    	# signal.signal(signal.SIGALRM, signal_handler)
	    	# signal.alarm(20)   # Ten seconds limit for Network restart
	    	
	    	temp_daemon_pid = "";
	    	for line in runProcess("service networking restart"):
	    		command_line = str(line)
	    		if command_line.find("pid") == 0:
	    			temp_damon_pid = command_line.split("=")[1]
	    			signal.signal(signal.SIGALRM, handler)
	    			signal.alarm(15)

	    		if command_line.find("DHCPDISCOVER") ==0:
	    			print('finded')
	    			count = count + 1
	    			
	    			if command_line.find("DHCPACK") == 0:
	    				signal.alarm(0)
	    				print('command_line',command_line)
	    				split_string = command_line.split("from ")
	    				gateway_ip = split_string[1]
	    				ip_set = True
	    				print("[ROOT] gateway_ip : ",gateway_ip)
	    	
	    	for line in runProcess('ifconfig wlan0'):
	    		command_line = str(line)
	    		if (command_line.find('cast:') == 36):
	    			split1 = command_line.split(' ')
	    			split2 = split1[11].split(':')
	    			ipaddr = split2[1]


	    	print('gw:',ipaddr)	
	    		
	    	if (is_network_alive(ipaddr)):
	    		p_sr.write(bytes("1", 'UTF-8'))
	    		print("[ROOT] Wi-Fi Connected")
	    	else:
	    		p_sr.write(bytes("2", 'UTF-8'))
	    		print("[ROOT] Failed to connect.")

	    	ap_mode = False

    except serial.SerialException as e:
        print('OS Error')
