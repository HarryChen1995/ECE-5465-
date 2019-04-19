#include "mbed.h"
 
SPISlave spi(D11, D12, D13, D10); // mosi, miso, sclk, ss
DigitalOut myled(LED1);
DigitalOut myled2(LED2);
//DigitalOut myled3(LED3);
 
int main() {
    Serial pc(USBTX, USBRX);
    pc.printf("This is a test\r\n");
    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 1MHz clock rate
    spi.format(8,3);
    spi.frequency(1000000);
    
    myled = 1;
    myled2 = 1;
    //myled3 = 1;
    
    char buffer[257];
    for(int i=0; i<256; i++){
        buffer[i]=0;
    }
    buffer[256] = '\0';
        
    while(1){
        pc.printf("Waiting for data\r\n");
        // Wait for a transfer to start
        while(!spi.receive()){};
    
        for(int i=0; i<256; i++){
            spi.reply(0x41);
            buffer[i]=spi.read(); //read data
        }

        pc.printf("RECV: %s\r\n", buffer);
        
        myled = !myled;
    }
}
