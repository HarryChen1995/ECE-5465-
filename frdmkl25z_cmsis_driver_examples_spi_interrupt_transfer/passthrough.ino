#include <SPI.h>

char buff[256];

void setup() {
  // Start serial port to get data
  // TODO: replace with ESP / Alexa stuff
  Serial.begin(9600);

  // Setup the SPI port
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV_8);
  digitalWrite(SS, HIGH);
}

void loop() {
  // Fill the buffer with zeros
  for(int i-0; i<256; i++){
    buff[i] = 0;
  }
  
  // Read in data from the serial port
  // TODO: Replace with however we get data from Alexa
  while (!Serial.available()) {}
  Serial.readBytes(buff, 256);

  // Pass data to board via SPI
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(SS, LOW);
  SPI.transfer(buff, 256); // If we are returning any data it is stored in buff as the data is written out
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
}
