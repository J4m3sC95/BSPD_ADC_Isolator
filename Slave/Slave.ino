/*
 * BSPD ADC Isolator:
 * - The function of this system is to gather sensor data from the BSPD sensors (brake pressure and current) and transmit it
 *    to the data-logger on NRX in such a way that the ground potentials of the datalogger and the bspd are isolated to avoid
 *    loops which if present could cause significant damage to the devices involved
 * - This is acheived with an ATTiny25 device collecting and measuring sensor data via its onboard ADC, this is then transmitted
 *    to the arduino acting as the datalogger via an optoisolator and a custom serial protocol
 * 
 * Maste Code:
 * - This is code will run on the ATTiny device
 */

void setup(){
}

void loop() {
}
