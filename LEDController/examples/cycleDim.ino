//Example of LEDController

#include <LEDController.h>


LEDController LED(9);


void loop()
{
	LED.cycleDim(1000,0,100);
}
