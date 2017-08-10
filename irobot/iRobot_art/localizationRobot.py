#!/usr/bin/python       #this is server file

from socket import *           #import socket module
import RPi.GPIO as GPIO
import time
import math
import numpy as np 

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(23, GPIO.OUT)
pwmLeft = GPIO.PWM(23, 100)

GPIO.setup(24, GPIO.OUT)
pwmRight = GPIO.PWM(24, 100)

DEFAULT_SPEED = 15.0

#Defines
default = 15         #in percentage for servo PWM
timeInterval = 0.1  #in time second 

#fwd
def goFwd():
    print "going straight"
    pwmLeft.ChangeDutyCycle(default + 2)
    pwmRight.ChangeDutyCycle(default - 2)
    


#right
def goRight():
    print "going right"
    pwmLeft.ChangeDutyCycle(default + 2)
    pwmRight.ChangeDutyCycle(default)
    


#left
def goLeft():
    print "going left"
    pwmLeft.ChangeDutyCycle(default)
    pwmRight.ChangeDutyCycle(default - 2)
    
#stall
def goStop():
    print "stop"
    pwmLeft.ChangeDutyCycle(0)
    pwmRight.ChangeDutyCycle(0)

#turn right
def turnRight():
    print "turning right"
    pwmLeft.ChangeDutyCycle(default + 1)
    pwmRight.ChangeDutyCycle(default)

#turn left
def turnLeft():
    print "turning left"
    pwmLeft.ChangeDutyCycle(default)
    pwmRight.ChangeDutyCycle(default - 1)

    
print "starting"

#initialize servo to be center
pwmLeft.start(DEFAULT_SPEED + 2)
pwmRight.start(DEFAULT_SPEED - 2)

#set up server
HOST = "192.168.137.100"
PORT = 8888
s = socket(AF_INET, SOCK_STREAM)
s.bind((HOST,PORT))

s.listen(1)
conn, addr = s.accept()
print "connected by: ", addr
count = 0

#conn.connect((HOST,PORT))
print "socket connected to server"
conn.sendall('Hello server')


while True:

    #receive command
    nav = conn.recv(1)
    print nav
    nav = ord(nav)
    print "Nav: ", nav
    speed = conn.recv(1)
    print speed
    speed = ord(speed)
    print "Speed: ", speed

    if(nav < 6):
        if (nav == 2 ): goLeft()
        elif (nav == 0 ): goRight()
        elif (nav == 1) : goFwd()
        elif(nav == 4): turnRight()
        elif(nav == 5): turnLeft()
        else : goStop()
                    

GPIO.cleanup()
