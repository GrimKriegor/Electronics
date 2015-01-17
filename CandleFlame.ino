//  Simulates the flickering of fire or a candle with single color LEDs //


//Led pin list
int CANDLES[] = {3, 5, 6};
int CANDLE_COUNT = sizeof(CANDLES) / sizeof(int);

//Sets LED pins in CANDLES[] to OUTPUT
void setup()
{
	for (int LED; LED < CANDLE_COUNT; LED++) {
		pinMode(CANDLES[LED], OUTPUT); 
	}
}

//Changes intensity of each LED
void loop()
{
	for (int LED; LED < CANDLE_COUNT; LED++) {
		analogWrite(CANDLES[LED], random(50,100)); 
	}
	delay(random(80,150));
}
