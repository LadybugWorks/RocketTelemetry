[Rocket Telemetry]
A week long telemetry project.
-------------------------------------------------
[Beagle Bone Black]
The beagle bone black (BBB) is a raspberry pi like development board.
I've also seen/heard of them being used at the companies I've worked at.

Originally, I was going to write a custom kernel.
However, I do have stuff to do, so I'm just going to use linux.

The beagle bone has a bunch of peripherals.
For this project, I'm just going to use the UART modules.
However, it would be cool to use CAN and have two controllers (that are cheaper than the BBB). One for sensor wrangling and one for communication.
-------------------------------------------------
[Camera]
The camera can be controlled with commands over UART.
Look at the RunCam Protocol here: https://support.runcam.com/hc/en-us/articles/360014537794-RunCam-Device-Protocol.
The camera's baudrate is 115200.
-------------------------------------------------
[Modems]
We are using two RFD900-US modems.
They are identified by the following names: ML and MB.
ML is the modem attached to the ground station laptop.
MB is the model attached to the beagle bone in the rocket.
They can communicate over a UART port and can be configured with AT commands. Use a 115200 baudrate and an FTDI cable for an easy way to configure them. You'll probably want an external 5V power supply. They do draw a lot of current even when not transmitting.

A quick note about AT commands:
To get into the command mode you need to send +++ and wait a second.
The modem will respond with OK\r\n. Then you can write commands.
Make sure you put a \r\n after each command but NOT after +++.
Any bytes sent less than a second after +++ will cause the radio to assume you meant to send data and not go into command mode.

Using the ATI5 command, I've compiled their settings below.
Sanity check these values if you're trying to do anything.
[ --- ML --- ]
S0:FORMAT=63
S1:SERIAL_SPEED=57
S2:AIR_SPEED=64
S3:NETID=25
S4:TXPOWER=30
S5:ECC=0
S6:MAVLINK=1
S7:OPPRESEND=0
S8:MIN_FREQ=902000
S9:MAX_FREQ=915000
S10:NUM_CHANNELS=51
S11:DUTY_CYCLE=100
S12:LBT_RSSI=0
S13:RTSCTS=0
S14:MAX_WINDOW=131
S15:ENCRYPTION_LEVEL=0
S16:GPI1_1R/CIN=0
S17:GPO1_1R/COUT=0
S18:GPO1_1SBUSIN=0
S19:GPO1_1SBUSOUT=0
S20:ANT_MODE=0
S21:GPO1_3STATLED=0
S22:GPO1_0TXEN485=0
S23:RATE/FREQBAND=0
S24:GPI1_2AUXIN=0
S25:GPO1_3AUXOUT=0
S26:AIR_FRAMELEN=120
S27:RSSI_IN_DBM=0
S28:FSFRAMELOSS=50
S29:AUXSER_SPEED=57
R0:TARGET_RSSI_dBm=0
R1:HYSTERESIS_RSSI_dBm=5 

[ --- MB --- ]
S0:FORMAT=63
S1:SERIAL_SPEED=57
S2:AIR_SPEED=64
S3:NETID=25
S4:TXPOWER=30
S5:ECC=0
S6:MAVLINK=1
S7:OPPRESEND=0
S8:MIN_FREQ=902000
S9:MAX_FREQ=915000
S10:NUM_CHANNELS=51
S11:DUTY_CYCLE=100
S12:LBT_RSSI=0
S13:RTSCTS=0
S14:MAX_WINDOW=131
S15:ENCRYPTION_LEVEL=0
S16:GPI1_1R/CIN=0
S17:GPO1_1R/COUT=0
S18:GPO1_1SBUSIN=0
S19:GPO1_1SBUSOUT=0
S20:ANT_MODE=0
S21:GPO1_3STATLED=0
S22:GPO1_0TXEN485=0
S23:RATE/FREQBAND=0
S24:GPI1_2AUXIN=0
S25:GPO1_3AUXOUT=0
S26:AIR_FRAMELEN=120
S27:RSSI_IN_DBM=0
S28:FSFRAMELOSS=50
S29:AUXSER_SPEED=57
R0:TARGET_RSSI_dBm=0
R1:HYSTERESIS_RSSI_dBm=5