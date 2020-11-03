/*
 * Book example, Needs work
 */

#include <Wire.h>
const byte slaveAddr = 0x57;
int registerAddr;                   // the shared register addr variable
int triggerPin = 9;
int echoPin = 10;
byte data[4];                       // the data registers 00 to 0x03

void setup() {
   pinMode(triggerPin, OUTPUT);     // the pin to send a 10us pulse
   pinMode(echoPin, INPUT);         // the response pin to measure
   TWBR=100000L;                    // set the i2c clk freq e.g. 400000L
   Wire.begin(slaveAddr);           // set up the Arduino as an I2C slave
   Wire.onReceive(receiveRegister); // register receive listener below
   Wire.onRequest(respondData);     // register respond listener below
}

void loop() {                       // loop 20 times per second
  int duration;                     // the response pulse width
  float distancecm, distancein;     // the converted value

  digitalWrite(triggerPin, HIGH);   // send the 10us pulse
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);  // measure response pulse (in us)

  distancecm = (float) duration / 58.0;    // time converted to cm
  data[0] = (int) distancecm;              // whole part (0x00)
  data[1] = (int) ((distancecm - data[0])*100); // fact part (0x01)
  distancein = (float) duration / 148.0;   // time converted to in
  data[2] = (int) distancein;              // whole part (0x02)
  data[3] = (int) ((distancein - data[2])*100); // fact part (0x03)

  delay(50);                        // 20 samples per second
}

void receiveRegister(int x){        // passes the number of bytes
  registerAddr = Wire.read();       // read in the one-byte address
}

void respondData(){                 // respond function
  byte dataValue = 0x00;            // default response value is 0x00
  if ((registerAddr >= 0x00) && (registerAddr <0x04)){
     dataValue = data[registerAddr];
  }
  Wire.write(dataValue);            // send the data back to the RPi
}
