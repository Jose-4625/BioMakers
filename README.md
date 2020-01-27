# BioMakers
## MS5540C.h
Library for MS5540C pressure sensor
* call MS5540C constructor in the setup portion of the arduino sketch
* Default Constructor has Pin definitions as follows:
	* MOSI = 11
	* MISO = 12
	* SCLK = 13
	* MCLK = 9
* run calibration function to retrieve calibration words and factors
* In new scripts with the same sensor:
	* assign calibration words 1-4 to cw1-4
	* call factorsFromWords function to create needed calibration factors
