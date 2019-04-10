#include "gpio.h"
#include "fsl_gpio.h"

using namspace std;

GPIO::GPIO(){

}

static int GPIO::open(int port, int DDR){

}

static int GPIO::close(int port){

}

static int GPIO::read(int port){

}

static void GPIO::write(int port, int value){

	GPIO_WritePinOutput(*base, pin, value);
}

GPIO::~GPIO(){

}
