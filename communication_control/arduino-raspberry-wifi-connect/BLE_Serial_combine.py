import bluetooth
import subprocess
import serial
import signal
from subprocess import call, PIPE, STDOUT
import subprocess
import time
import shlex
import os




#Serial config 
Serial_Location_port = "/dev/ttyACM0"
Serial_wpa_configure_file = "/etc/wpa.config"

#Bluetooth config
BT_ap_mode = False

def get_return_code_of_simple_cmd(cmd, stderr=STDOUT):
    """Execute a simple external command and return its exit status."""
    args = shlex.split(cmd)
    return call(args, stdout=PIPE, stderr=stderr)

def is_network_alive(gateway_ip):
	cmd = "ping -c 1 " + gateway_ip
	return get_return_code_of_simple_cmd(cmd) == 0

def errorHandler(signum, stack):
    raise IOError("")

def runProcess(p): #execute System command
	yield "pid=" + str(os.getpgid(p.pid))
	while(True):
		retcode = p.poll() #returns None while subprocess is running
		line = p.stdout.readline()
		yield line
		if(retcode is not None):
			networkProc_pid = ""
			break

def BluetoothReceiver():
	p = subprocess.Popen("sudo hciconfig hci0 piscan", stdout=subprocess.PIPE, stderr=subprocess.STDOUT,shell=True)
	# Init bluetooth for receive
	server_sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
	port = 1
	global BT_ap_mode
	BT_ap_mode = False
	server_sock.bind(("",port))
	server_sock.listen(1)
	print("listening bluetooth signal from Client...")

#	try:
	client_sock, address = server_sock.accept()
	print("Accepted connection from ", address)
	while 1:
		data = client_sock.recv(1024)
		split_data = str(data).split("'")[1]
		command_str = split_data.split('\\r\\n')[0]
		bluetoothHandler(command_str, client_sock)

#	except Exception as e:
		#print("Error", e)
		#client_sock.close()
		#server_sock.close()

def bluetoothHandler(value, client_sock):
	global BT_ap_mode
	if value == "wifi":
		client_sock.send("Sent from Handler")
		print(dir(client_sock))
		print("Wi-Fi Mode Enabled")
		BT_ap_mode = True

	print(value, BT_ap_mode)

	if BT_ap_mode == True and not(value == "wifi"):
		process = value.split("/")
		print(process)
		print("AP Set process..")
		print("---------------------------------")
		result = call(["wpa_passphrase", process[0], process[1]])
		print("[ROOT] Saving configure file")
		call("wpa_passphrase " + process[0] + " " + process[1] + "> " + Serial_wpa_configure_file ,shell=True)
		call("ifdown wlan0", shell=True)
		p = subprocess.Popen("ifup wlan0", stdout=subprocess.PIPE, stderr=subprocess.STDOUT,shell=True,preexec_fn=os.setsid)
		try:
			for line in runProcess(p):

				command_line = str(line)

				if command_line.find("pid") == 0:
					signal.signal(signal.SIGALRM, errorHandler)
					signal.alarm(15)

				if command_line.find("DHCPDISCOVER") ==0:
					print('finded')
					count = count + 1
					if command_line.find("DHCPACK") == 0:
						
						print('command_line',command_line)
						split_string = command_line.split("from ")
						gateway_ip = split_string[1]
						ip_set = True
						print("[ROOT] gateway_ip : ",gateway_ip)

			p = subprocess.Popen("ifconfig wlan0", stdout=subprocess.PIPE, stderr=subprocess.STDOUT,shell=True,preexec_fn=os.setsid)
			for line in runProcess(p):
				command_line = str(line)
				if (command_line.find('cast:') == 36):
					split1 = command_line.split(' ')
					split2 = split1[11].split(':')
					ipaddr = split2[1]

			#ipaddr = gateway Address

			if (is_network_alive(ipaddr)): # ping to gateway address (alive check)
				signal.alarm(0) # Alarm Off
				client_sock.send("[BT] Wi-Fi Connected")
				print("[ROOT] Wi-Fi Connected")
			else:
				client_sock.send("[BT] Failed to connect")
				print("[ROOT] Wi-Fi Connected")


		except IOError: #Limit 15 Seconds
			os.killpg(os.getpgid(p.pid), signal.SIGTERM)
			print("Process Terminiated by Timeout (15 seconds)")
			ap_mode = False

		ap_mode = False


def SerialReceiver():
	p_sr = serial.Serial(Serial_Location_port,9600)
	p_sr.flushInput()
	while True:
		try:
			debug = str(p_sr.readline())
			test = debug.replace(r"\r\n" ,"")
			test3 = test.replace("b'", "")
			SerialString = test3.replace("'" , "")
			print(SerialString)
		except:
			print("")

BluetoothReceiver()
#SerialReceiver()
