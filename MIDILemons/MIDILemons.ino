/*
	:: MIDILemon Arduino library ::
	by Grim Kriegor <grimkriegor@opmbx.org>
	Version 150125
*/

#include <CapacitiveSensor.h>
#include <MIDI.h>
#include <Arduino.h>



class MIDILemons
{
	private:
		int _SENDERPIN;
		int _RECEIVERPIN;
		int _SENSITIVITY;
		int _NOTE;
		long _INPUT;
		long _NEWINPUT;
		long _OLDINPUT;
	public:
		CapacitiveSensor SENSOR;
		MIDILemons(int _SENDERPIN, int _RECEIVERPIN, int _SENSITIVITY, int _NOTE);
		void MIDIMessage(int _COMMAND, int _NOTE, int _VELOCITY);
		void noteOn();
		void noteOff();
		boolean check();
};	
		
MIDILemons::MIDILemons(int _SENDERPIN, int _RECEIVERPIN, int _SENSITIVITY, int _NOTE) : SENSOR(_SENDERPIN, _RECEIVERPIN) {}

void MIDILemons::MIDIMessage(int _COMMAND, int _NOTE, int _VELOCITY) {
	Serial.write(_COMMAND);
	Serial.write(_NOTE);
	Serial.write(_VELOCITY);
	delay(50);
}

void MIDILemons::noteOn() {
	MIDIMessage(0x90, _NOTE, 0x14);
}

void MIDILemons::noteOff() {
	MIDIMessage(0x90, _NOTE, 0x00);
}

boolean MIDILemons::check() {
	boolean _STATUS;
	long _INPUT = SENSOR.capacitiveSensor(30);
	
	static long _OLDINPUT;
	long _NEWINPUT = _INPUT;
	
	//Serial.write(_INPUT);
	//Serial.write(_OLDINPUT);
	
	if ((_OLDINPUT < _SENSITIVITY) && (_NEWINPUT > _SENSITIVITY)) { Serial.write("ON"); }
	if ((_OLDINPUT > _SENSITIVITY) && (_NEWINPUT < _SENSITIVITY)) { Serial.write("OFF"); }
	
	return _STATUS;
		
}
	
MIDILemons LEMON1(2, 3, 200, 0x24);

void setup()
{
	//Serial.begin(38400);
	Serial.begin(9600);
}

void loop()
{
	LEMON1.check();
}
