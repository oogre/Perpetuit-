#include <AccelStepper.h>

//AccelStepper Xaxis(1, 2, 5); // pin 2 = step, pin 5 = direction
//AccelStepper Yaxis(1, 3, 6); // pin 3 = step, pin 6 = direction
//AccelStepper Zaxis(1, 4, 7); // pin 4 = step, pin 7 = direction

AccelStepper Xaxis(1, 11, 10); // pin 3 = step, pin 6 = direction

void setup() {
  Xaxis.setMaxSpeed(1000);
  Xaxis.setSpeed(1000);
  Xaxis.setAcceleration(250);
  Xaxis.move(-10000);//36800
}

void loop() {  
   Xaxis.run();
}
