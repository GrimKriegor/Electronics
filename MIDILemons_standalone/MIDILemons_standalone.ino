//
// :: MIDILemons Arduino Drumkit ::
//
// Detects lemon hits via capacitive sensing and
// sends MIDI information via USB serial
//
// Grim Kriegor <grimkriegor@krutt.org>
//
//
// Circuit suggestion:
//  * Digital pin 2 as sender
//  * Digital pins 3, 4 and 5 as receivers
//  * Receivers connected to sender via 1M ohm resistors
//  * Sensor connected directly to the receivers
//

#include <CapacitiveSensor.h>
#include <MIDI.h>

//Configure sender pin
#define SENDERPIN 12

//Configure sensitivity
#define SENSITIVITY 200


//Configure MIDI Notes
const int DRUM_KICK = 0x24; 
const int DRUM_SNARE = 0x26; 
const int DRUM_CRASH = 0x31;
const int DRUM_HIHATS = 0x2A;
const int DRUM_RIDE = 0x2B;
const int DRUM_CRASH2 = 0x39;


//Configure LEMON notes
const int LEMON1_NOTE = DRUM_CRASH;
const int LEMON2_NOTE = DRUM_SNARE;
const int LEMON3_NOTE = DRUM_KICK;
const int LEMON4_NOTE = DRUM_CRASH2;
const int LEMON5_NOTE = DRUM_KICK;
const int LEMON6_NOTE = DRUM_KICK;
const int LEMON7_NOTE = DRUM_KICK;
const int LEMON8_NOTE = DRUM_KICK;
const int LEMON9_NOTE = DRUM_KICK;
const int LEMON10_NOTE = DRUM_KICK;
const int LEMON11_NOTE = DRUM_KICK;
const int LEMON12_NOTE = DRUM_KICK;


//MIDI codes, velocity, and noteON
const int ON = 0x14;
const int OFF = 0x00;
const int noteON = 0x90;


//Define sensor objects
CapacitiveSensor LEMON1 = CapacitiveSensor(SENDERPIN, 2);
CapacitiveSensor LEMON2 = CapacitiveSensor(SENDERPIN, 3);
CapacitiveSensor LEMON3 = CapacitiveSensor(SENDERPIN, 4);
CapacitiveSensor LEMON4 = CapacitiveSensor(SENDERPIN, 5);
CapacitiveSensor LEMON5 = CapacitiveSensor(SENDERPIN, 6);
CapacitiveSensor LEMON6 = CapacitiveSensor(SENDERPIN, 7);
CapacitiveSensor LEMON7 = CapacitiveSensor(SENDERPIN, 8);
CapacitiveSensor LEMON8 = CapacitiveSensor(SENDERPIN, 9);
CapacitiveSensor LEMON9 = CapacitiveSensor(SENDERPIN, 10);
CapacitiveSensor LEMON10 = CapacitiveSensor(SENDERPIN, 11);
CapacitiveSensor LEMON11 = CapacitiveSensor(SENDERPIN, A1);
CapacitiveSensor LEMON12 = CapacitiveSensor(SENDERPIN, A2);


//Function to send MIDI message
void MIDImessage(int command, int note, int velocity) {
	Serial.write(command); //Note ON or OFF
	Serial.write(note); //Pitch
	Serial.write(velocity); //Velocity
	delay(30);
}


void setup()
{
	//Set MIDI baud rate:
	Serial.begin(38400);
}


void loop()
{
	long LEMON_TIMING1  = LEMON1.capacitiveSensor(30);
	long LEMON_TIMING2  = LEMON2.capacitiveSensor(30);
	long LEMON_TIMING3  = LEMON3.capacitiveSensor(30);
	long LEMON_TIMING4  = LEMON4.capacitiveSensor(30);
	long LEMON_TIMING5  = LEMON5.capacitiveSensor(30);
	long LEMON_TIMING6  = LEMON6.capacitiveSensor(30);
	long LEMON_TIMING7  = LEMON7.capacitiveSensor(30);
	long LEMON_TIMING8  = LEMON8.capacitiveSensor(30);
	long LEMON_TIMING9  = LEMON9.capacitiveSensor(30);
	long LEMON_TIMING10 = LEMON10.capacitiveSensor(30);
	long LEMON_TIMING11 = LEMON11.capacitiveSensor(30);
	long LEMON_TIMING12 = LEMON12.capacitiveSensor(30);


	//Check LEMON1
	static int LAST_INPUT1 = 0;
	int NEW_INPUT1 = LEMON_TIMING1;
	if((LAST_INPUT1 < SENSITIVITY) && (NEW_INPUT1 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON1_NOTE, ON); };
	if((LAST_INPUT1 > SENSITIVITY) && (NEW_INPUT1 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON1_NOTE, OFF); };
	LAST_INPUT1 = NEW_INPUT1;
		
	//Check LEMON2
	static int LAST_INPUT2 = 0;
	int NEW_INPUT2 = LEMON_TIMING2;
	if((LAST_INPUT2 < SENSITIVITY) && (NEW_INPUT2 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON2_NOTE, ON); };
	if((LAST_INPUT2 > SENSITIVITY) && (NEW_INPUT2 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON2_NOTE, OFF); };
	LAST_INPUT2 = NEW_INPUT2;
		
	//Check LEMON3
	static int LAST_INPUT3 = 0;
	int NEW_INPUT3 = LEMON_TIMING3;
	if((LAST_INPUT3 < SENSITIVITY) && (NEW_INPUT3 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON3_NOTE, ON); };
	if((LAST_INPUT3 > SENSITIVITY) && (NEW_INPUT3 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON3_NOTE, OFF); };
	LAST_INPUT3 = NEW_INPUT3;
		
	//Check LEMON4
	static int LAST_INPUT4 = 0;
	int NEW_INPUT4 = LEMON_TIMING4;
	if((LAST_INPUT4 < SENSITIVITY) && (NEW_INPUT4 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON4_NOTE, ON); };
	if((LAST_INPUT4 > SENSITIVITY) && (NEW_INPUT4 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON4_NOTE, OFF); };
	LAST_INPUT4 = NEW_INPUT4;
		
	//Check LEMON5
	static int LAST_INPUT5 = 0;
	int NEW_INPUT5 = LEMON_TIMING5;
	if((LAST_INPUT5 < SENSITIVITY) && (NEW_INPUT5 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON5_NOTE, ON); };
	if((LAST_INPUT5 > SENSITIVITY) && (NEW_INPUT5 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON5_NOTE, OFF); };
	LAST_INPUT5 = NEW_INPUT5;
		
	//Check LEMON6
	static int LAST_INPUT6 = 0;
	int NEW_INPUT6 = LEMON_TIMING6;
	if((LAST_INPUT6 < SENSITIVITY) && (NEW_INPUT6 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON6_NOTE, ON); };
	if((LAST_INPUT6 > SENSITIVITY) && (NEW_INPUT6 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON6_NOTE, OFF); };
	LAST_INPUT6 = NEW_INPUT6;
		
	//Check LEMON7
	static int LAST_INPUT7 = 0;
	int NEW_INPUT7 = LEMON_TIMING7;
	if((LAST_INPUT7 < SENSITIVITY) && (NEW_INPUT7 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON7_NOTE, ON); };
	if((LAST_INPUT7 > SENSITIVITY) && (NEW_INPUT7 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON7_NOTE, OFF); };
	LAST_INPUT7 = NEW_INPUT7;
		
	//Check LEMON8
	static int LAST_INPUT8 = 0;
	int NEW_INPUT8 = LEMON_TIMING8;
	if((LAST_INPUT8 < SENSITIVITY) && (NEW_INPUT8 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON8_NOTE, ON); };
	if((LAST_INPUT8 > SENSITIVITY) && (NEW_INPUT8 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON8_NOTE, OFF); };
	LAST_INPUT8 = NEW_INPUT8;
	
	//Check LEMON9
	static int LAST_INPUT9 = 0;
	int NEW_INPUT9 = LEMON_TIMING9;
	if((LAST_INPUT9 < SENSITIVITY) && (NEW_INPUT9 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON9_NOTE, ON); };
	if((LAST_INPUT9 > SENSITIVITY) && (NEW_INPUT9 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON9_NOTE, OFF); };
	LAST_INPUT9 = NEW_INPUT9;
		
	//Check LEMON10
	static int LAST_INPUT10 = 0;
	int NEW_INPUT10 = LEMON_TIMING10;
	if((LAST_INPUT10 < SENSITIVITY) && (NEW_INPUT10 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON10_NOTE, ON); };
	if((LAST_INPUT10 > SENSITIVITY) && (NEW_INPUT10 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON10_NOTE, OFF); };
	LAST_INPUT10 = NEW_INPUT10;

		
	//Check LEMON11
	static int LAST_INPUT11 = 0;
	int NEW_INPUT11 = LEMON_TIMING11;
	if((LAST_INPUT11 < SENSITIVITY) && (NEW_INPUT11 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON11_NOTE, ON); };
	if((LAST_INPUT11 > SENSITIVITY) && (NEW_INPUT11 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON11_NOTE, OFF); };
	LAST_INPUT11 = NEW_INPUT11;


	//Check LEMON12
	static int LAST_INPUT12 = 0;
	int NEW_INPUT12 = LEMON_TIMING12;
	if((LAST_INPUT12 < SENSITIVITY) && (NEW_INPUT12 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON12_NOTE, ON); };
	if((LAST_INPUT12 > SENSITIVITY) && (NEW_INPUT12 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON12_NOTE, OFF); };
	LAST_INPUT12 = NEW_INPUT12;
}
