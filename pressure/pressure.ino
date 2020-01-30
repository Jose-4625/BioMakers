#include "ms5540c.h"

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  static MS5540C snr;

  snr.calibration();
  snr.measure();
  delay(1000);
  
}
