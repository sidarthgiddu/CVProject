#!/usr/bin/python       #this is server file

# *********************************************************************
#  *
#  *                  RaspberryPi Robot Box Control 
#  *
#  *********************************************************************
#  Cornell University Computer Systems Laboratory
#  2016
 
#  Items:
#  RaspberryPi Model B+ with Raspbian OS 
#  2 Parallex Continuous Servos [1.3 mSec, 1.7 mSec] range with 10 mSec pulse
#  Dual-port Power Bank
#  3D printed robot frame

#  Code Summary:
#  initialize RPi I/O 
#  Set up server
#  Get data from client (speed, angle)
#  Scale data to servo range
#  PWM 

# Resources:
# [Servo]: https://www.parallax.com/sites/default/files/downloads/900-00008-Continuous-Rotation-Servo-Documentation-v2.2.pdf
# [Client, Server Network]: http://www.tutorialspoint.com/python/python_networking.htm
# [3D printed frame]: https://www.thingiverse.com/thing:13042

#  *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from socket import *           #import socket module
import RPi.GPIO as GPIO
import time
import math
import numpy as np 

# initialize RPi pinout. pin16 (GPIO23) [Left] and pin18 (GPIO24) [Right]
# initialize PWM cycle w/ 100 Hz 
# change PWM duty cycle by percentage to fit [1.3, 1.7] mSec range
GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.OUT)
pwmLeft = GPIO.PWM(23, 100)
GPIO.setup(24, GPIO.OUT)
pwmRight = GPIO.PWM(24, 100)

# mid-range results to no movement 
DEFAULT_SPEED = 15.0


# sample basic functions for robot movement
#fwd
def goFwd():
    pwmLeft.ChangeDutyCyclestart(17)
    pwmRight.ChangeDutyCyclestart(13)
    time.sleep(5)

#back
def goBack():
    pwmLeft.ChangeDutyCycle(13)
    pwmRight.ChangeDutyCycle(17)
    time.sleep(5)

# convert read-friendly range of [0-255] speed and [-90,90] angle to servo PWM range
def scaleRange(oldValue, oldMin, oldMax, newMin, newMax):
     
    scaledValue = ((oldValue - oldMin) * (newMax-newMin))/(oldMax-oldMin) + newMin
    return scaledValue


#initialize servo to be center
pwmLeft.start(DEFAULT_SPEED)
pwmRight.start(DEFAULT_SPEED)

#set up server
HOST = "192.168.137.120" #MODIFY FOR EACH ROBOT
PORT = 23
s = socket(AF_INET, SOCK_STREAM)
s.bind((HOST,PORT))

s.listen(1)
conn, addr = s.accept()
print "connected by: ", addr
count = 0

# main loop to get data and covert to servo movements
while True:

	#receive command 2 bytes (speed, angle)
	data = conn.recv(2)
	speed = ord(data[0])
	angleIn = ord(data[1])
	angle = int(angleIn) - (1<<8) #twos complement convert for signed value

    #every other data is valid so accept when count ==0, discard when count == 1
	if (count ==0):
            
            # Left angle results in non-positive value, values hardcoded from trial and error
            if(angle <= -167):
                angle = scaleRange(angle, -256, -167, 1, 89) 


            # Interpretted the received data
            print "speed: ", speed, "angle: " , angle

            
            # scale [0-255] value to servo range [13,17]
            # servo range is 4 mSec, with 15 mSec being middle
            scaledValue = scaleRange(speed, 0,255,-2.,2.)

            # go L by slowing left
            if (angle >= 10 ):
                    print "left"
                    pwmRight.ChangeDutyCycle(DEFAULT_SPEED - scaledValue)
                    
                    tempValue = scaledValue*abs(math.sin(angle*np.pi/180.))
                    
                    pwmLeft.ChangeDutyCycle(DEFAULT_SPEED + tempValue)

            # go R by slowing right
            elif (angle <= -10 ):
                    print "right"
                    pwmLeft.ChangeDutyCycle(DEFAULT_SPEED + scaledValue)
                    
                    tempValue = scaledValue*abs(math.sin(angle*np.pi/180.))
                    
                    pwmRight.ChangeDutyCycle(DEFAULT_SPEED - tempValue)

            # go straight
            else:
                    print "straight"
                    pwmLeft.ChangeDutyCycle(DEFAULT_SPEED + scaledValue)
                    pwmRight.ChangeDutyCycle(DEFAULT_SPEED - scaledValue)

            count = 1

        # discard data
        else:
            count = 0

