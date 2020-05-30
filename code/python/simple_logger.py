import serial
import syslog
import time

#The following line is for serial over GPIO
# port = '/dev/ttyS0'
port = '/dev/tty.wchusbserial1420'
baud = 9600
recording_t = 30
ard = serial.Serial(port,115200,timeout=5)
time.sleep(2) #wait for Arduino

start_t = time.time()
with open("data_logger/output.csv", mode="w+") as out_file:
    out_file.truncate(0)
    while(time.time() - start_t < 30):
	    # Serial read section
        cur_count = ard.inWaiting()
        if(cur_count != 0):
            msg_b = ard.read(cur_count) # read all characters in buffer
            msg_s = msg_b.decode("ascii")
            if(len(msg_s) > 6):
                print (msg_s)
                out_file.write(msg_s)
print("End of programme")
    #separate errors into list [timestamp, force error, current force error, velocity error, current velocity error]
