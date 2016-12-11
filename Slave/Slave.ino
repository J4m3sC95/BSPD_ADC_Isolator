/*
 * BSPD ADC Isolator:
 * - The function of this system is to gather sensor data from the BSPD sensors (brake pressure and current) and transmit it
 *    to the data-logger on NRX in such a way that the ground potentials of the datalogger and the bspd are isolated to avoid
 *    loops which if present could cause significant damage to the devices involved
 * - This is acheived with an ATTiny25 device collecting and measuring sensor data via its onboard ADC, this is then transmitted
 *    to the arduino acting as the datalogger via an optoisolator and a custom serial protocol
 *    
 * Custom Serial Protocol:
 * - Idle state: Clock Pin = HIGH, Data Pin = HIGH
 * - Start communication: Clock pin goes low, wait for Data Pin to drop
 * - Data: data is triggered on the rising edge of the clock and should be sampled by the datalogging arduino on the falling 
 *     edge of the clock
 * - 3 x 2 byte integers will be trasmitted corresponsing to the current sensor, rear brake pressure and front brake pressure 
 *    respectively, the data will be transmitted LSB first
 * 
 * Master Code:
 * - This is the code that will run on the ATTiny device
 */

 // pin definitions for BSPD communication (change 0 to desired number)
#define CLOCK_PIN 0
#define DATA_PIN  0

#define CURRENT_SENSOR_PIN        0
#define PRESSURE_SENSOR_REAR_PIN  0
#define PRESSURE_SENSOR_FRONT_PIN 0

unsigned int BSPD_data[] = {0,0,0};

void setup(){
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DATA_PIN, OUTPUT);  
  digitalWrite(DATA_PIN, HIGH);
}

void loop() {
  // wait for the clock to go low to indicate the start of a transmission
  while(digitalRead(CLOCK_PIN));
  // collect adc readings
  BSPD_data[0] = analogRead(CURRENT_SENSOR_PIN);
  BSPD_data[1] = analogRead(PRESSURE_SENSOR_REAR_PIN);
  BSPD_data[2] = analogRead(PRESSURE_SENSOR_FRONT_PIN);

  // lower the data_pin to indicate ready for communication
  digitalWrite(DATA_PIN, LOW);

  for(int n = 0; n < 3; n ++){
    for(int m = 0; m < 16; m++){
      // wait for the clock signal to rise
      while(!digitalRead(CLOCK_PIN));
      if(BSPD_data[n] & (1 << m)){
        digitalWrite(DATA_PIN, HIGH);
      }
      else{
        digitalWrite(DATA_PIN, LOW);
      }
      // wait until the clock pin to drop
      while(digitalRead(CLOCK_PIN));
    }
  }
  // raise data pin
  digitalWrite(DATA_PIN, HIGH);
}
