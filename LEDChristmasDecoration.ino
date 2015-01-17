//   Simple Christmas Decoration using single color LEDs  //
//  4 Modes: Normal order, Reverse order, Shuffle, Flash  //


//LED pin list
int LEDS[] = {3, 5, 6};
int LEDS_COUNT = sizeof(LEDS) / sizeof(int);

//Initialize LED pins as OUTPUT
void setup()
{
	for (int LED; LED < LEDS_COUNT; LED++) {
		pinMode(LEDS[LED], OUTPUT); }
	randomSeed(analogRead(0));
}


//Turns the LED on and then off, DELAY between on and off
void lightLed(int LED, int DELAY) {
	digitalWrite(LED, HIGH);
	delay(DELAY);
	digitalWrite(LED, LOW);
	delay(DELAY);
}

//Turns all LEDs on and then off, DELAY between on and off
void lightAllLeds(int DELAY) {
	for (int LED; LED < LEDS_COUNT; LED++) { digitalWrite(LEDS[LED], HIGH); }
	delay(DELAY);
	for (int LED; LED < LEDS_COUNT; LED++) { digitalWrite(LEDS[LED], LOW); }
	delay(DELAY);
}

//Fades LED on and off with PWM, DELAY between each change in intensity
void dimLed(int LED, int DELAY) {
	analogWrite(LED, 0);
	for (int i=0; i < 100; i++) { analogWrite(LED, i); delay(DELAY); }
	for (int i=100; i > 0; i--) { analogWrite(LED, i); delay(DELAY); }
	digitalWrite(LED, LOW);
}


//In each loop the MODE gets sorted and the DELAY randomized
void loop()
{
	int MODE = random(0,4);
	int DELAY = random(0.01,5);
		
	int TIMES = random(1,5);
	int FLASH_TIME = random(10,500);


	switch(MODE) {
		case 0: //Normal Order
			for (int LED; LED < LEDS_COUNT; LED++) {
				dimLed(LEDS[LED], DELAY); }
				
		case 1: //Reverse Order
			for (int LED=LEDS_COUNT; LED > 0; LED--) {
				dimLed(LEDS[LED], DELAY); }

		case 2: //Flash all LEDs for TIMES
			for (int i; i < TIMES; i++) {
				lightAllLeds(FLASH_TIME); }
				
		case 3: //Shuffle
			for (int i; i <= LEDS_COUNT; i++) {
				int LED = random(1, LEDS_COUNT+1);
				dimLed(LEDS[LED], DELAY); }			
	}
	
} //End of main loop
