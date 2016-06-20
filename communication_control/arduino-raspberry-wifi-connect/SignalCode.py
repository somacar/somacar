import subprocess
import os
import signal

def runProcess(p): #execute System command
        yield "pid=" + str(os.getpgid(p.pid))
        while(True):
                retcode = p.poll() #returns None while subprocess is running
                line = p.stdout.readline()
                yield line
                if(retcode is not None):
                        networkProc_pid = ""
                        break

def errorHandler(signum, stack):
    raise IOError("Couldn't open device!")

# Set the signal handler and a 5-second alarm
# This open() may hang indefinitely
try:
	p = subprocess.Popen("ping 127.0.0.1 -c 3", stdout=subprocess.PIPE, stderr=subprocess.STDOUT,shell=True,preexec_fn=os.setsid)
	for line in runProcess(p):
		command_line = str(line)
		if command_line.find("pid") == 0:
			proc_pid = command_line.split("=")[1]
			print("list")
			init_proc_pid = proc_pid
			signal.signal(signal.SIGALRM, errorHandler)
			signal.alarm(5)
		
		print(line)
	
except IOError:
	os.killpg(os.getpgid(p.pid), signal.SIGTERM)
	print("Process Terminiated by Timeout (5 seconds)")

except OSError:
	print("OS ErorrT>T")
	

signal.alarm(0)          # Disable the alarm
