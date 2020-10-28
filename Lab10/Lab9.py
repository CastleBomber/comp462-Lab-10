#!/usr/bin/env python
import RPi_I2C_driver, time
import datetime
import smbus
import os
import time

bus = smbus.SMBus(1)
address = 0x68
CONV = 32

def convTemp(address):
    byte_control = bus.read_byte_data(address,0x0E)
    if byte_control&CONV == 0:
        bus.write_byte_data(address, 0x0E, byte_control|CONV)
    byte_control = bus.read_byte_data(address,0x0E)
    while byte_control&CONV != 0:
        time.sleep(1)
        byte_control = bus.read_byte_data(address,0x0E)
    return True

def getTemp(address):
    convTemp(address)
    byte_tmsb = bus.read_byte_data(address,0x11)
    byte_tlsb = bus.read_byte_data(address,0x12)
    tinteger = (byte_tmsb & 0x7f) + ((byte_tmsb & 0x80) >> 7) * -2**8
    tdecimal = (byte_tmsb >> 7) * 2**(-1) + ((byte_tmsb & 0x40) >> 6) * 2**(-2)
    return tinteger + tdecimal

def main():
    Celsius = getTemp(address)
    mylcd = RPi_I2C_driver.lcd()

    while(True):
        mylcd.lcd_display_string("Time: " + str(datetime.datetime.now().time()), 1)
        mylcd.lcd_display_string("Temp: "+ str(Celsius) + " C", 2)
        time.sleep(1)
        #mylcd.lcd_clear()
        #mylcd.backlight(0)

main()
