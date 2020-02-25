import serial
import syslog
import time
import numpy as np
import matplotlib as plt

#The following line is for serial over GPIO
port = '/dev/ttyS0'
time = []
forceErrors = []
forceCurrentErrors = []
velocityErrors = []
velocityCurrentErrors = []
fig, axs = plt.subplots(2, 2)
axs[0, 0].plot(x, y)
axs[0, 0].set_title('Axis [0, 0]')
axs[0, 1].plot(x, y, 'tab:orange')
axs[0, 1].set_title('Axis [0, 1]')
axs[1, 0].plot(x, -y, 'tab:green')
axs[1, 0].set_title('Axis [1, 0]')
axs[1, 1].plot(x, -y, 'tab:red')
axs[1, 1].set_title('Axis [1, 1]')

axs.flat[0].set(xlabel='time', y-label='force error')
axs.flat[1].set(xlabel='time', y-label='current force error')
axs.flat[2].set(xlabel='time', y-label='velocity error')
axs.flat[3].set(xlabel='time', y-label='current velocity error')

axs.flat[0].plot(time, forceErrors)
axs.flat[1].plot(time, forceCurrentErrors)
axs.flat[2].plot(time, velocityErrors)
axs.flat[3].plot(time, velocityCurrentErrors)





ard = serial.Serial(port,9600,timeout=5)
time.sleep(2) #wait for Arduino

for i in range(0, 100):
	# Serial read section
    msg = ard.read(ard.inWaiting()) # read all characters in buffer
    print ("Message from arduino: ")
    print (msg)
    currErrors = msg.split(",") #separate errors into list [timestamp, force error, current force error, velocity error, current velocity error]
    time.append(currErrors[0])
    forceErrors.append(currErrors[1])
    forceCurrentErrors.append(currErrors[2])
    velocityErrors.append(currErrors[3])
    velocityCurrentErrors.append(currErrors[4])


