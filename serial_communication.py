import serial
import time
from serial.serialutil import EIGHTBITS, PARITY_NONE, STOPBITS_ONE, SerialException
try: 
    ser = serial.Serial('COM3', 9600, timeout = 2, stopbits = STOPBITS_ONE, bytesize = EIGHTBITS, parity = PARITY_NONE)
    ser.flushInput()
    ser.flushOutput()
except SerialException:
    print("Can't load serial port. Exiting.")
    exit()
while True:
    print("Enter voltage: ")
    try:
        inputstring = input()
        if inputstring == 'f': 
            ser.flushInput()
            ser.flushOutput()
            print('Flushed.')
            continue
        num = float(inputstring)
        if num > 5:
            num = 5
        elif num < 0:
            num = 0
    except ValueError:
        print("Bad value. Exiting.")
        break
    outstring = f"{num:.2f}!".encode(encoding="utf-8")
    try:
        sent = ser.write(outstring)
        print(f'Sent {sent} characters')
        strin = ser.read(size=4)
        if outstring[:-1] != strin : 
            print('Sending failed.') 
            ser.flushInput()
            ser.flushOutput()
    except SerialException:
        print("Sending failed. Exiting.")
        break
    print(f"Sent: {outstring}, received: {strin} ")
    time.sleep(2)
ser.close()
print("End of program.")