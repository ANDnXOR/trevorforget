# Trevor Forget

This repository hosts our small contribution to Trevor's memory. 

# Building

To build the code, Atmel Studio is required. For the 2018 Trevor put it into the ATTiny816 mode. For 2019 Trevor switch to ATTiny3217.

# Flashing

This requires an Atmel Ice due to the prorprietary UPDI protocol. Although there is an open source project pyupdi that we have tested once and it worked: [https://github.com/mraardvark/pyupdi].

The 2019 Trevor utilzes a Tag Connect 2050 and adapter to map the 3.3v, gnd, and UPDI pins properly.
