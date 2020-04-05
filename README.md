# EM4100 RFID spoofer
Spoof a known RFID-ID with a selfmade spoofer based on an arduino and an inductive coil.

 > ## EM4100_RIFD_spoofer  
 > Quote source(Transmitter): https://github.com/Einstein2150/EM4100_RFID_spoofer  
 > Quote source(Receiver): https://github.com/ifsc-arliones/RDM6300-Arduino  
 > modified by: 7M4MON, 9 Mar 2020  
 > 
 > I added some functions :  
 > + Marge RDM6300 reader and EM4100 Transmitter.  
 > + Save and Load with build-in EEPOM (Max 10 IDs)  
 > + Display ID on LCD (I2C 8x2, AQM0802A)  
 > + Convert from numeric ID to send bit array with parity.  

![](https://github.com/7m4mon/EM4100_RFID_spoofer/blob/master/em4100_spoofer_arduino.jpg)

##Hardware-Requirements
- Arduino nano (or compatible)
- 0.1uF capacitor (104)
- NPN transistor (i.e. 2222A)
- 10k ohm resistor
- PCB Board
- 26 Gauge wire (.0179 inches / 0,40 mm)

##The Coil
**Make a coil** which has a inductance of 16 uH. For example:
- 53 turns
- 0.511811 inches dia coil
- 0.0179 inches wire

makes a inductance of 15.60240931881926 uH.

**Tune your coil** to 125 kHz. For example:
- 0.1uF capacitor
- 16 uH inductive coil

makes a frequency of 125000 Hz.

##Web links for calculating the coil
[Coil Calculator](http://www.crystalradio.net/cal/indcal2.shtml) for Calculating the inductance of the coil.

[Frequency Calculator](http://www.sengpielaudio.com/Rechner-XLC.htm) for tuning the Coil to 125 kHz

##The Code
###General

You have to set your spoofing-ID in the `data_to_spoof[114]`-Array.
- ID is starting with nine `1`
- it follows a half-byte (4 bit) followed by a XOR bit
	- 20 half-byte and 20 XOR bit makes your 10 byte ID
- at last 4 XOR bit (for each column) and a final `0` stop bit

![EM4100 dataformat](https://github.com/Einstein2150/EM4100_RFID_spoofer/blob/master/images/EM4100_dataformat.png)

###Replace the ID
- In the code you have to replace this. Places for the half bytes are marked with `id`-placeholder.
- Places for the XOR-Calculation are marked with `xor`-placeholder.

![EM4100 example](https://github.com/Einstein2150/EM4100_RFID_spoofer/blob/master/images/EM4100_example.png)

**Without replacing the ID to a correct value you can't compile the code!**

##Building the spoofer
1. Connect the 10k ohm resistor on arduino pin 9
2. Connect the resistor to the base of the NPN transistor
3. Connect the coil and the capacitor parallel on emitter and diode of the transistor
4. Connect one foot of the capacitor to ground

![PCB](https://github.com/Einstein2150/EM4100_RFID_spoofer/blob/master/images/Build_on_PCB.png)

![Breadboard](https://github.com/Einstein2150/EM4100_RFID_spoofer/blob/master/images/Build_on_Breadboard.png)

##Spoofer in action

The spoofer in action: https://www.youtube.com/watch?v=GybcqNRcsuM

##Contributes
The project which inspired me:
http://www.instructables.com/id/Stupid-Simple-Arduino-LF-RFID-Tag-Spoofer