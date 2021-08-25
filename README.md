# PIC18F45K22-project
Simple project which demonstrates serial communication, DAC and ADC done as team project in subject Fundamentals of Digital Electronics on **EasyPIC v7** development board equipped with **PIC18F45K22** microcontroller.


#### Python communication program

- The program uses pySerial library.
- The program initially establishes connection with the COM3 serial port. Then flushes input and output buffers so that they would not affect the sending/receiving of data later. 
- Execution of the program is cyclic, where the user is first asked for the voltage value which is then processed. 
- If the value is not valid, the program exits. 
- If the value of the program is **f**, the program again flushes input and output buffers in case of communication problems. 
- If the value is correct (float), it is then limited to DAC range (0V - 5V) and is sent as series of characters to the microcontroller. 
- As a confirmation, the  microcontroller should send the same message back. The program waits 2 seconds before next transmission.


#### microC program for microcontroller

- In the main program, the ports are first configured.
- The AD converter is set to input **RA0** (**AN0**), references set to internal signals, alignment to left, clock to 2MHz, sampling time to 4 TAD.
- Then the LCD is initialized, bits for interrupts and AD as well as DA conversion on **RE3** (**CCP5**) starts.
- In the infinite loop, it is checked whether new data for setting voltage has arrived, and if so, it is accepted character by character. During that transmission, ADC interrupt is disabled.
- When the transfer is completed, the same data is sent back as confirmation and duty cycle is set on DA converter once more, as well as AD conversion.
- The newValue flag indicates whether the newly accepted voltage should be showed on LCD which, again, disables and then enables interrupts.
- The last part of the code refers to the printing of the read value from the AD converter to the LCD which is done by hand. While doing so, interrupts are again disabled and then enabled. Using tick counter, the screen update is limited to a short period of time. 
- Interrupt routine serves the AD converter, extracts the value from ADRESH register and discards the lowest two bits from ADRESL register because that much precision is not necessary. It then restarts the conversion.
