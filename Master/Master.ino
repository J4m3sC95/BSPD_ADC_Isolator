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
 * - This is example code to be implemented on the master device (datalogger) of the BSPD ADC Isolator system
 */

// pin definitions for BSPD communication (change 0 to desired number)
#define CLOCK_PIN 0
#define DATA_PIN  0
// the half period of the clock in microseconds (can possibly be 0)
#define CLOCK_DELAY 50

// array to store the data from the bspd
unsigned int BSPD_data_array[4] = {0,0,0};

void setup(){
  BSPD_setup();
}

void loop() {
  BSPD_get_data(BSPD_data_array);
}

void BSPD_setup(){
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(CLOCK_PIN, HIGH);
  pinMode(DATA_PIN, INPUT);  
}

void BSPD_get_data(unsigned int *data){
  // initialise the data array
  for(int n = 0; n < 3; n++){
    data[n] = 0;
  }
  // Start Communication
  // set clock pin low to start communication
  digitalWrite(CLOCK_PIN, LOW);
  // wait for data line to go low
  while(digitalRead(DATA_PIN));

  // read data
  // loop through each integer
  for(int n = 0; n < 3; n++){
    // loop through each bit of the integer
    for(int m = 0; m < 16; m++){
      // raise the clock and pause before reading the value
      digitalWrite(CLOCK_PIN, HIGH);
      delayMicroseconds(CLOCK_DELAY);
      data[n] += digitalRead(DATA_PIN) << m;
      digitalWrite(CLOCK_PIN, LOW);
      delayMicroseconds(CLOCK_DELAY);
    }
  }

  // set clock high to indicate the transmission is finished
  digitalWrite(CLOCK_PIN, HIGH);
}

