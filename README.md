# BioMakers
## MS5540C.h
Library for MS5540C pressure sensor
* call MS5540C constructor in the setup portion of the arduino sketch
* Default Constructor has Pin definitions as follows:
	* MOSI = 11
	* MISO = 12
	* SCLK = 13
	* MCLK = 9
* All Calibration is done at initialization
* measure(int printOut = 0, int raw = 0)
	* printOut = 1; Auto prints to Serial
	* raw = 1; Auto prints all raw data to Serial
* calibration(int readOut = 0)
	* readOut = 1; Auto prints all calibration data to Serial
* Useful attributes
	* pcompreal => real pressure with compensation (mbar)
	* pcomphg => real pressure with compensation (mmhg)
	* temp => real temperature with compensation (C)
### Usage best practices
```
#include "ms5540c.h"
void setup(){

}
void loop(){
	static MS5540C snr;	// static allows for fixed memory allocation of the MS5540C object
  snr.calibration();
  snr.measure();

	// snr.pcompreal/pcomphg/temp depending on data needed
	Serial.println(snr.pcompreal)

}

```
