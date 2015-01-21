//  Simulates the flickering of fire or a candle with single color LEDs //


//Led pin list
int CANDLES[] = {9, 10, 11};
int CANDLE_COUNT = sizeof(CANDLES) / sizeof(int);

//Configure Intensity, or tune it via analog input
int INTENSITY_MAX = 100;
int INTENSITY_MIN = 50;
boolean INTENSITY_AJUST = true; //Set to true to set the intensity via analog input

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
	if (INTENSITY_AJUST) { 
		INTENSITY_MAX = map(analogRead(0), 0, 670, 5, 100); 
		INTENSITY_MIN = map(analogRead(0), 0, 670, 2, 50); }
	
	for (int LED; LED < CANDLE_COUNT; LED++) {
		analogWrite(CANDLES[LED], random(INTENSITY_MIN, INTENSITY_MAX)); 
	}
	delay(random(80,150));
}
