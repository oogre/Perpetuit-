/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */


import processing.serial.*;

Serial [] myPorts = new Serial[2];
PVector p;
float r = 1.0f/255;

void setup() 
{
  size(800, 600);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  p = new PVector();
  /* master */  myPorts[0] = new Serial(this, "/dev/cu.wchusbserial1420", 9600);
  /* slave */  myPorts[1] = new Serial(this, "/dev/cu.usbmodem1411", 9600);
  background(255);
}


void serialEvent(Serial thisPort) {
  // variable to hold the number of the port:
  int portNumber = -1;

  // iterate over the list of ports opened, and match the 
  // one that generated this event:
  for (int p = 0; p < myPorts.length; p++) {
    if (thisPort == myPorts[p]) {
      portNumber = p;
    }
  }
  // read a byte from the port:
  int value = thisPort.read();
  if(portNumber == 0){
    p.x = (value * r * 2) -1;
  }
  else if(portNumber == 1){
    p.y = (value * r * 2) -1;
  }
}

void draw()
{
  
  stroke(0);
  translate(width>>1, height>>1);
  point(p.x * 350, p.y * 100);
}
