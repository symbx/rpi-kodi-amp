#include "pt2322.h"
/*#include <unistd.h>*/

PT2322::PT2322(Wire* wire) {
	this->_wire = wire;
	this->_functions = 0;
}

int PT2322::writeByte(char b) {
	this->_wire->begin(ADDRESS);
	int r = this->_wire->write(b);
	this->_wire->end();
	return  r;
}

/*void PT2322::initialize() {
	usleep(300000); //300ms
	this->reset();
	this->inputSW();

	this->setMasterVolume(0);
	this->setFunction3D(0);

	this->setVolumeTrim_FL(0);
	this->setVolumeTrim_FR(0);
	this->setVolumeTrim_CT(0);
	this->setVolumeTrim_RL(0);
	this->setVolumeTrim_RR(0);
	this->setVolumeTrim_SW(0);

	this->setTone_Bass(7);
	this->setTone_Middle(7);
	this->setTone_Treble(7);
}*/

int PT2322::reset() {
	return this->writeByte(SYS_RESET);
}

void PT2322::setMasterVolume(char vol) {
	int res = (vol / 10) << 4;
	res = res | (vol % 10);
	this->_wire->begin(ADDRESS);
	this->_wire->write((byte) (VOL_1STEP | (res & 0x0f)));
	this->_wire->write((byte) (VOL_10STEP | ((res & 0xf0) >> 4)));
	this->_wire->end();
}

void PT2322::setVolumeTrim(char chan, char trim) {
	if(trim > 15)
		trim = 15;
	if(trim < 0)
		trim = 0;
	trim = (char) (15 - trim);
	trim = trim | chan;
	this->writeByte(trim);
}

/*void PT2322::setVolumeTrim_FL(char trim) {
	this->setVolumeTrim(VOL_TRIM_FL, trim);
}

void PT2322::setVolumeTrim_FR(char trim) {
	this->setVolumeTrim(VOL_TRIM_FR, trim);
}

void PT2322::setVolumeTrim_CT(char trim) {
	this->setVolumeTrim(VOL_TRIM_CT, trim);
}

void PT2322::setVolumeTrim_RL(char trim) {
	this->setVolumeTrim(VOL_TRIM_RL, trim);
}

void PT2322::setVolumeTrim_RR(char trim) {
	this->setVolumeTrim(VOL_TRIM_RR, trim);
}

void PT2322::setVolumeTrim_SW(char trim) {
	this->setVolumeTrim(VOL_TRIM_SW, trim);
}*/

void PT2322::setTone(char type, char tone) {
	if(tone > 15)
		tone = 15;
	if(tone < 0)
		tone = 0;
	tone = (char) (15 - tone);
	tone = tone | type;
	this->writeByte(tone);
}

void PT2322::setTone_Bass(char tone) {
	this->setTone(TONE_BASS, tone);
}

void PT2322::setTone_Middle(char tone) {
	this->setTone(TONE_MIDDLE, tone);
}

void PT2322::setTone_Treble(char tone) {
	this->setTone(TONE_TREBLE, tone);
}

void PT2322::inputSW() {
	this->writeByte(INPUT_SW);
}

void PT2322::setFunctionTone(char on) {
	if(on)
		this->_functions &= (0x0f - FUNC_TONE);
	else
		this->_functions |= FUNC_TONE;
	this->writeByte(FUNCTION | this->_functions);
}

void PT2322::setFunction3D(char on) {
	if(on)
		this->_functions &= (0x0f - FUNC_3D);
	else
		this->_functions |= FUNC_3D;
	this->writeByte(FUNCTION | this->_functions);
}

void PT2322::setFunctionMute(char on) {
	if(on)
		this->_functions |= FUNC_MUTE;
	else
		this->_functions &= (0x0f - FUNC_MUTE);
	this->writeByte(FUNCTION | this->_functions);
}