//#include <SPI.h>

int MOSI = 11;
int MISO = 12;
int SCK = 13;
float wait_time = 1.0 / 9600.0;

void setup() {
  Serial.begin(9600);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  digitalWrite(SCK, HIGH);
}

void loop() {
  // Read in data from the PC application
  while (!Serial.available()) {}
  char buff[256];
  Serial.readBytes(buff, 256);

  // Pass data to board via bit-bang SPI
  send_spi(buff, 256);
}

int send_spi(char* buff, int len) {
  // Iterate over bytes
  int i;
  for(i=0; i < len; i++){
    // Iterate over bits
    int b;
    for(b=0; b < 8; b+=2){
      digitalWrite(SCK, LOW);
      digitalWrite(MOSI, (buff[i] >> b) & 0x01);
      digitalWrite(SCK, HIGH);
      digitalWrite(MOSI, (buff[i] >> (b+1)) & 0x01);
    }
  }
}