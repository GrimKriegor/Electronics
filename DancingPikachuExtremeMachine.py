#####################################################
#
# Dancing Pikachu Extreme Machine
# https://www.youtube.com/watch?v=IP1p2O8ddiY
# 2014 November
#
#####################################################

import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BOARD)

#SETUP LED GPIO
GPIO.setup(11, GPIO.OUT)
led = GPIO.PWM(11,50)
GPIO.setup(12, GPIO.OUT)
led1 = GPIO.PWM(12,50)

#SETUP BUZZER GPIO
GPIO.setup(22, GPIO.OUT)
GPIO.output(22, 0)

#SETUP SERVO INSTANCE
GPIO.setup(16, GPIO.OUT)
servo = GPIO.PWM(16,50)
servo.start(50)


#FLASH AND DIM LED
def dim(output, delay):
    output.start(0)
    for i in range(100):
        output.ChangeDutyCycle(i)
        time.sleep(delay)
    for i in range(100):
        output.ChangeDutyCycle(100-i)
        time.sleep(delay)
    output.stop()

#BUZZ
def buzz(pin, delay):
    GPIO.output(pin, GPIO.HIGH)
    time.sleep(delay)
    GPIO.output(pin, GPIO.LOW)

#TURN SERVO
def turn(side):
    dutycycle = { "left" : 12.5, "right" : 2.5 }
    servo.ChangeDutyCycle(dutycycle[side])
    time.sleep(1)

#CHECK LIGHT LEVEL
def RCtime(RCpin):
    reading = 0
    GPIO.setup(RCpin, GPIO.OUT)
    GPIO.output(RCpin, 0)
    time.sleep(.1)

    GPIO.setup(RCpin, GPIO.IN)
    while (GPIO.input(RCpin) == GPIO.LOW):
        reading += 1
    return reading


try:
    while True:
        DARK = True if RCtime(7) > 100 else False

        if (DARK):
            dim(led, 0.005)
            buzz(22, 0.005)
            turn("right")
            dim(led1, 0.005)
            buzz(22, 0.005)
            turn("left")

except KeyboardInterrupt:
    pass

GPIO.cleanup()

