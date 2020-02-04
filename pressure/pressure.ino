#include "ms5540c.h"

void setup() {
  Serial.begin(9600);

}
void loop() {
  // put your main code here, to run repeatedly:
  static MS5540C snr;

  snr.calibration();
  snr.measure();

  Serial.println(snr.pcompreal);
  Serial.println(snr.pcomphg);
  delay(1000);

}
