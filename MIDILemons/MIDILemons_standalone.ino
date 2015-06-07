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
CapacitiveSensor lemon1 = CapacitiveSensor(SENDERPIN, 2);
CapacitiveSensor lemon2 = CapacitiveSensor(SENDERPIN, 3);
CapacitiveSensor lemon3 = CapacitiveSensor(SENDERPIN, 4);
CapacitiveSensor lemon4 = CapacitiveSensor(SENDERPIN, 5);
CapacitiveSensor lemon5 = CapacitiveSensor(SENDERPIN, 6);
CapacitiveSensor lemon6 = CapacitiveSensor(SENDERPIN, 7);
CapacitiveSensor lemon7 = CapacitiveSensor(SENDERPIN, 8);
CapacitiveSensor lemon8 = CapacitiveSensor(SENDERPIN, 9);
CapacitiveSensor lemon9 = CapacitiveSensor(SENDERPIN, 10);
CapacitiveSensor lemon10 = CapacitiveSensor(SENDERPIN, 11);
CapacitiveSensor lemon11 = CapacitiveSensor(SENDERPIN, A1);
CapacitiveSensor lemon12 = CapacitiveSensor(SENDERPIN, A2);





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
	long total1  = lemon1.capacitiveSensor(30);
	long total2  = lemon2.capacitiveSensor(30);
	long total3  = lemon3.capacitiveSensor(30);
	long total4  = lemon4.capacitiveSensor(30);
	long total5  = lemon5.capacitiveSensor(30);
	long total6  = lemon6.capacitiveSensor(30);
	long total7  = lemon7.capacitiveSensor(30);
	long total8  = lemon8.capacitiveSensor(30);
	long total9  = lemon9.capacitiveSensor(30);
	long total10 = lemon10.capacitiveSensor(30);
	long total11 = lemon11.capacitiveSensor(30);
	long total12 = lemon12.capacitiveSensor(30);


	//Check LEMON1
	static int lastInput1 = 0;
	int newInput1 = total1;
	if((lastInput1 < SENSITIVITY) && (newInput1 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON1_NOTE, ON); };
	if((lastInput1 > SENSITIVITY) && (newInput1 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON1_NOTE, OFF); };
	lastInput1 = newInput1;
		
	//Check LEMON2
	static int lastInput2 = 0;
	int newInput2 = total2;
	if((lastInput2 < SENSITIVITY) && (newInput2 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON2_NOTE, ON); };
	if((lastInput2 > SENSITIVITY) && (newInput2 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON2_NOTE, OFF); };
	lastInput2 = newInput2;
		
	//Check LEMON3
	static int lastInput3 = 0;
	int newInput3 = total3;
	if((lastInput3 < SENSITIVITY) && (newInput3 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON3_NOTE, ON); };
	if((lastInput3 > SENSITIVITY) && (newInput3 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON3_NOTE, OFF); };
	lastInput3 = newInput3;
		
	//Check LEMON4
	static int lastInput4 = 0;
	int newInput4 = total4;
	if((lastInput4 < SENSITIVITY) && (newInput4 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON4_NOTE, ON); };
	if((lastInput4 > SENSITIVITY) && (newInput4 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON4_NOTE, OFF); };
	lastInput4 = newInput4;
		
	//Check LEMON5
	static int lastInput5 = 0;
	int newInput5 = total5;
	if((lastInput5 < SENSITIVITY) && (newInput5 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON5_NOTE, ON); };
	if((lastInput5 > SENSITIVITY) && (newInput5 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON5_NOTE, OFF); };
	lastInput5 = newInput5;
		
	//Check LEMON6
	static int lastInput6 = 0;
	int newInput6 = total6;
	if((lastInput6 < SENSITIVITY) && (newInput6 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON6_NOTE, ON); };
	if((lastInput6 > SENSITIVITY) && (newInput6 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON6_NOTE, OFF); };
	lastInput6 = newInput6;
		
	//Check LEMON7
	static int lastInput7 = 0;
	int newInput7 = total7;
	if((lastInput7 < SENSITIVITY) && (newInput7 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON7_NOTE, ON); };
	if((lastInput7 > SENSITIVITY) && (newInput7 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON7_NOTE, OFF); };
	lastInput7 = newInput7;
		
	//Check LEMON8
	static int lastInput8 = 0;
	int newInput8 = total8;
	if((lastInput8 < SENSITIVITY) && (newInput8 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON8_NOTE, ON); };
	if((lastInput8 > SENSITIVITY) && (newInput8 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON8_NOTE, OFF); };
	lastInput8 = newInput8;
	
	//Check LEMON9
	static int lastInput9 = 0;
	int newInput9 = total9;
	if((lastInput9 < SENSITIVITY) && (newInput9 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON9_NOTE, ON); };
	if((lastInput9 > SENSITIVITY) && (newInput9 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON9_NOTE, OFF); };
	lastInput9 = newInput9;
		
	//Check LEMON10
	static int lastInput10 = 0;
	int newInput10 = total10;
	if((lastInput10 < SENSITIVITY) && (newInput10 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON10_NOTE, ON); };
	if((lastInput10 > SENSITIVITY) && (newInput10 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON10_NOTE, OFF); };
	lastInput10 = newInput10;

		
	//Check LEMON11
	static int lastInput11 = 0;
	int newInput11 = total11;
	if((lastInput11 < SENSITIVITY) && (newInput11 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON11_NOTE, ON); };
	if((lastInput11 > SENSITIVITY) && (newInput11 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON11_NOTE, OFF); };
	lastInput11 = newInput11;


	//Check LEMON12
	static int lastInput12 = 0;
	int newInput12 = total12;
	if((lastInput12 < SENSITIVITY) && (newInput12 > SENSITIVITY)) {
		MIDImessage(noteON, LEMON12_NOTE, ON); };
	if((lastInput12 > SENSITIVITY) && (newInput12 < SENSITIVITY)) {
		MIDImessage(noteON, LEMON12_NOTE, OFF); };
	lastInput12 = newInput12;
}
