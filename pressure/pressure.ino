#include "ms5540c.h"


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  static MS5540C snr;
  /*snr.c1 = 21848;
  snr.c2 = 2125;
  snr.c3 = 718;
  snr.c4 = 609;
  snr.c5 = 1028;
  snr.c6 = 26;*/
  snr.calibration();
  snr.measure();
  delay(1000);

}
