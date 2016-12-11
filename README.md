# BSPD_ADC_Isolator
Provides an isolated ADC for the BSPD sensors to interface with the datalogger on Newcastle Racing's NRX 

BSPD ADC Isolator:
- The function of this system is to gather sensor data from the BSPD sensors (brake pressure and current) and transmit it
   to the data-logger on NRX in such a way that the ground potentials of the datalogger and the bspd are isolated to avoid
   loops which if present could cause significant damage to the devices involved
- This is acheived with an ATTiny25 device collecting and measuring sensor data via its onboard ADC, this is then transmitted
   to the arduino acting as the datalogger via an optoisolator and a custom serial protocol
   
Custom Serial Protocol:
- Idle state: Clock Pin = HIGH, Data Pin = HIGH
- Start communication: Clock pin goes low, wait for Data Pin to drop
- Data: data is triggered on the rising edge of the clock and should be sampled by the datalogging arduino on the falling 
    edge of the clock
- 3 x 2 byte integers will be trasmitted corresponsing to the current sensor, rear brake pressure and front brake pressure 
   respectively, the data will be transmitted LSB first
   
This repo includes Arduino code for both:
- The Master board which will be the data-logging arduino
- The Slave board which will be the ATTiny25 on the BSPD
