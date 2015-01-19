//
// :: MIDILemons Arduino Drumkit ::
//
// Detects lemon hits via capacitive sensing and
// sends MIDI information via USB serial
//
// Grim Kriegor <grimkriegor@opmbx.org>
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
#define SENDERPIN 2

//Configure sensitivity
#define SENSITIVITY 200

//Configure MIDI Notes
const int lemon1_note = 0x24; //Drum Kick
const int lemon2_note = 0x26; //Rock Snare
const int lemon3_note = 0x31; //Crash

//MIDI codes, velocity, and noteON
const int ON = 0x14;
const int OFF = 0x00;
const int noteON = 0x90;

//Define sensor objects
CapacitiveSensor lemon1 = CapacitiveSensor(SENDERPIN, 3);
CapacitiveSensor lemon2 = CapacitiveSensor(SENDERPIN, 4);
CapacitiveSensor lemon3 = CapacitiveSensor(SENDERPIN, 5);


//Function to send MIDI message
void MIDImessage(int command, int note, int velocity) {
	Serial.write(command); //Note ON or OFF
	Serial.write(note); //Pitch
	Serial.write(velocity); //Velocity
	delay(50);
}


void setup()
{
	//Set MIDI baud rate:
	Serial.begin(38400);
}


void loop()
{
	long total1 = lemon1.capacitiveSensor(30);
	long total2 = lemon2.capacitiveSensor(30);
	long total3 = lemon3.capacitiveSensor(30);


	//Check LEMON1
	static int lastInput1 = 0;
	int newInput1 = total1;
	if((lastInput1 < SENSITIVITY) && (newInput1 > SENSITIVITY)) {
		MIDImessage(noteON, lemon1_note, ON); };
	if((lastInput1 > SENSITIVITY) && (newInput1 < SENSITIVITY)) {
		MIDImessage(noteON, lemon1_note, OFF); };
	lastInput1 = newInput1;
		
	//Check LEMON2
	static int lastInput2 = 0;
	int newInput2 = total2;
	if((lastInput2 < SENSITIVITY) && (newInput2 > SENSITIVITY)) {
		MIDImessage(noteON, lemon2_note, ON); };
	if((lastInput2 > SENSITIVITY) && (newInput2 < SENSITIVITY)) {
		MIDImessage(noteON, lemon2_note, OFF); };
	lastInput2 = newInput2;
		
	//Check LEMON3
	static int lastInput3 = 0;
	int newInput3 = total3;
	if((lastInput3 < SENSITIVITY) && (newInput3 > SENSITIVITY)) {
		MIDImessage(noteON, lemon3_note, ON); };
	if((lastInput3 > SENSITIVITY) && (newInput3 < SENSITIVITY)) {
		MIDImessage(noteON, lemon3_note, OFF); };
	lastInput3 = newInput3;
	
}
