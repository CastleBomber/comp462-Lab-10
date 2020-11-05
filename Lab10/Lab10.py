#!/usr/bin/env python
import RPi_I2C_driver, time
import datetime
import smbus
import os
import time

bus = smbus.SMBus(1)
address = 0x68



def main():
    mylcd = RPi_I2C_driver.lcd()

    while(True):
        mylcd.lcd_display_string("Distance: " )
        mylcd.lcd_display_string("Distance: ")
        time.sleep(1)
        #mylcd.lcd_clear()
        #mylcd.backlight(0)

main()
