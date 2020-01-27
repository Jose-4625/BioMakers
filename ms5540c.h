/*
 MS5540C Pressure Sensor
 Once you read out the calibration factors you can define them in the header of any sketch you write for the sensor.
*/


// include library:
#include <SPI.h>

class MS5440C{
  public:
    // Calibration words 1-4
    long cw1;
    long cw2;
    long cw3;
    long cw4;
    // Calibration factors 1-6
    long c1;
    long c2;
    long c3;
    long c4;
    long c5;
    long c6;
    // Temperature
    int rawtemp;
    int temp;
    // Pins
    int MOSI; // Din
    int MISO; // Dout
    int SCK;
    int MCLK;
    
    void resetSensor();
    void temperature();
    void rawPressure();
    void compPressure();
    void secondDegCompPressure();
    void calibration();
    MS5440C(int MOSI = 11, int MISO = 12, int SCK = 13, int MCLK = 9); //constructor

};

// default constructor
MS5440C::MS5440C(){
  MS5440C(int MOSI = 11, int MISO = 12, int SCK = 13, int MCLK = 9);
}
// Parameterized constructor
MS5440C::MS5440C(int MOSI = 11, int MISO = 12, int SCK = 13, int MCLK = 9) {
  MOSI = MOSI;
  MISO = MISO;
  SCK = SCK;
  MCLK = MCLK;
  Serial.begin(9600);
  SPI.begin(); //see SPI library details on arduino.cc for details
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV32); //divide 16 MHz to communicate on 500 kHz
  pinMode(MCLK, OUTPUT);
  delay(100);
}

void MS5440C::resetSensor() //this function keeps the sketch a little shorter
{
  SPI.setDataMode(SPI_MODE0);
  SPI.transfer(0x15);
  SPI.transfer(0x55);
  SPI.transfer(0x40);
}

void MS5440C::calibration()
{
  TCCR1B = (TCCR1B & 0xF8) | 1 ; //generates the MCKL signal
  analogWrite (MCLK, 128) ;

  resetsensor(); //resets the sensor - caution: afterwards mode = SPI_MODE0!

  //Calibration word 1
  unsigned int result1 = 0;
  unsigned int inbyte1 = 0;
  SPI.transfer(0x1D); //send first byte of command to get calibration word 1
  SPI.transfer(0x50); //send second byte of command to get calibration word 1
  SPI.setDataMode(SPI_MODE1); //change mode in order to listen
  result1 = SPI.transfer(0x00); //send dummy byte to read first byte of word
  result1 = result1 << 8; //shift returned byte
  inbyte1 = SPI.transfer(0x00); //send dummy byte to read second byte of word
  result1 = result1 | inbyte1; //combine first and second byte of word
  Serial.print("Calibration word 1 = ");
  Serial.print(result1,HEX);
  Serial.print(" "); 
  Serial.println(result1);
  cw1 = result1; 

  resetsensor(); //resets the sensor

  //Calibration word 2; see comments on calibration word 1
  unsigned int result2 = 0;
  byte inbyte2 = 0;
  SPI.transfer(0x1D);
  SPI.transfer(0x60);
  SPI.setDataMode(SPI_MODE1);
  result2 = SPI.transfer(0x00);
  result2 = result2 <<8;
  inbyte2 = SPI.transfer(0x00);
  result2 = result2 | inbyte2;
  Serial.print("Calibration word 2 = ");
  Serial.print(result2,HEX); 
  Serial.print(" "); 
  Serial.println(result2);
  cw2 = result2 

  resetsensor(); //resets the sensor

  //Calibration word 3; see comments on calibration word 1
  unsigned int result3 = 0;
  byte inbyte3 = 0;
  SPI.transfer(0x1D);
  SPI.transfer(0x90);
  SPI.setDataMode(SPI_MODE1);
  result3 = SPI.transfer(0x00);
  result3 = result3 <<8;
  inbyte3 = SPI.transfer(0x00);
  result3 = result3 | inbyte3;
  Serial.print("Calibration word 3 = ");
  Serial.print(result3,HEX); 
  Serial.print(" "); 
  Serial.println(result3); 
  cw3 = result3
  resetsensor(); //resets the sensor

  //Calibration word 4; see comments on calibration word 1
  unsigned int result4 = 0;
  byte inbyte4 = 0;
  SPI.transfer(0x1D);
  SPI.transfer(0xA0);
  SPI.setDataMode(SPI_MODE1);
  result4 = SPI.transfer(0x00);
  result4 = result4 <<8;
  inbyte4 = SPI.transfer(0x00);
  result4 = result4 | inbyte4;
  Serial.print("Calibration word 4 = ");
  Serial.print(result4,HEX);
  Serial.print(" "); 
  Serial.println(result4); 
  cw4 = result4

  //now we do some bitshifting to extract the calibration factors
  //out of the calibration words;
  c1 = (result1 >> 1) & 0x7FFF;
  c2 = ((result3 & 0x003F) << 6) | (result4 & 0x003F);
  c3 = (result4 >> 6) & 0x03FF;
  c4 = (result3 >> 6) & 0x03FF;
  c5 = ((result1 & 0x0001) << 10) | ((result2 >> 6) & 0x03FF);
  c6 = result2 & 0x003F;

  Serial.print("c1 = ");
  Serial.println(c1);
  Serial.print("c2 = ");
  Serial.println(c2);
  Serial.print("c3 = ");
  Serial.println(c3);
  Serial.print("c4 = ");
  Serial.println(c4);
  Serial.print("c5 = ");
  Serial.println(c5);
  Serial.print("c6 = ");
  Serial.println(c6);

  resetsensor(); //resets the sensor
}
void MS5440C::rawPressure(void){
    unsigned int presMSB = 0; //first byte of value
    unsigned int presLSB = 0; //last byte of value
    unsigned int D1 = 0;
    SPI.transfer(0x0F); //send first byte of command to get pressure value
    SPI.transfer(0x40); //send second byte of command to get pressure value
    delay(35); //wait for conversion end
    SPI.setDataMode(SPI_MODE1); //change mode in order to listen
    presMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
    presMSB = presMSB << 8; //shift first byte
    presLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
    D1 = presMSB | presLSB; //combine first and second byte of value
    Serial.print("D1 - Pressure raw = ");
    Serial.println(D1);

    resetsensor(); //resets the sensor
  }
   

void MS5440C::temperature(){
  unsigned int tempMSB = 0; //first byte of value
  unsigned int tempLSB = 0; //last byte of value
  unsigned int D2 = 0;
  SPI.transfer(0x0F); //send first byte of command to get temperature value
  SPI.transfer(0x20); //send second byte of command to get temperature value
  delay(35); //wait for conversion end
  SPI.setDataMode(SPI_MODE1); //change mode in order to listen
  tempMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
  tempMSB = tempMSB << 8; //shift first byte
  tempLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
  D2 = tempMSB | tempLSB; //combine first and second byte of value
  Serial.print("D2 - Temperature raw = ");
  Serial.println(D2); //voila!
  rawtemp = D2;
}
void MS5440C::compPressure(){
  //calculation of the real values by means of the calibration factors and the maths
  //in the datasheet. const MUST be long
  const long UT1 = (c5 << 3) + 20224;
  const long dT = D2 - UT1;
  const long TEMP = 200 + ((dT * (c6 + 50)) >> 10);
  const long OFF  = (c2 * 4) + (((c4 - 512) * dT) >> 12);
  const long SENS = c1 + ((c3 * dT) >> 10) + 24576;
  const long X = (SENS * (D1 - 7168) >> 14) - OFF;
  long PCOMP = ((X * 10) >> 5) + 2500;
  float TEMPREAL = TEMP/10;
  float PCOMPHG = PCOMP * 750.06 / 10000; // mbar*10 -> mmHg === ((mbar/10)/1000)*750/06

  Serial.print("Real Temperature in C = ");
  Serial.println(TEMPREAL);

  Serial.print("Compensated pressure in mbar = ");
  Serial.println(PCOMP);
  Serial.print("Compensated pressure in mmHg = ");
  Serial.println(PCOMPHG);
}
void MS5440C::secondDegCompPressure(){
  //2-nd order compensation only for T < 20°C or T > 45°C
 
  long T2 = 0;
  float P2 = 0;

  if (TEMP < 200)
    {
      T2 = (11 * (c6 + 24) * (200 - TEMP) * (200 - TEMP) ) >> 20;
      P2 = (3 * T2 * (PCOMP - 3500) ) >> 14;
    }
  else if (TEMP > 450)
    {
      T2 = (3 * (c6 + 24) * (450 - TEMP) * (450 - TEMP) ) >> 20;
      P2 = (T2 * (PCOMP - 10000) ) >> 13;   
    }

  if ((TEMP < 200) || (TEMP > 450))
  {
    const float TEMP2 = TEMP - T2;
    const float PCOMP2 = PCOMP - P2;

    float TEMPREAL2 = TEMP2/10;
    float PCOMPHG2 = PCOMP2 * 750.06 / 10000; // mbar*10 -> mmHg === ((mbar/10)/1000)*750/06

    Serial.print("2-nd Real Temperature in C = ");
    Serial.println(TEMPREAL2);

    Serial.print("2-nd Compensated pressure in mbar = ");
    Serial.println(PCOMP2);
    Serial.print("2-nd Compensated pressure in mmHg = ");
    Serial.println(PCOMPHG2);
  }
}