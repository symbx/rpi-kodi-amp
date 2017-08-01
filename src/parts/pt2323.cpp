#include "pt2323.h"

PT2323::PT2323(Wire* wire) {
	this->_wire = wire;
}

void PT2323::writeByte(char b) {
	this->_wire->begin(ADDRESS);
	this->_wire->write(b);
	this->_wire->end();
}

/*void PT2323::initialize() {
	this->setInput(INPUT_C6);
}*/

void PT2323::setInput(char line) {
	this->writeByte(SET_INPUT | line);
}

/*void PT2323::setInput_L1() {
	this->setInput(INPUT_L1);
}

void PT2323::setInput_L2() {
	this->setInput(INPUT_L2);
}

void PT2323::setInput_L3() {
	this->setInput(INPUT_L3);
}

void PT2323::setInput_L4() {
	this->setInput(INPUT_L4);
}

void PT2323::setInput_C6() {
	this->setInput(INPUT_C6);
}*/

void PT2323::setSurround(char on) {
	if(on > 1)
		on = 1;
	this->writeByte(SURROUND | on);
}

void PT2323::setMixChannel(char on) {
	if(on > 1)
		on = 1;
	this->writeByte(MIX_CHANNEL | on);
}

void PT2323::setMute(char chan, char on) {
	if(on > 1)
		on = 1;
	this->writeByte(MUTE | chan | on);
}

/*void PT2323::setMute_FL(char on) {
	this->setMute(MUTE_FL, on);
}

void PT2323::setMute_FR(char on) {
	this->setMute(MUTE_FR, on);
}

void PT2323::setMute_CT(char on) {
	this->setMute(MUTE_CT, on);
}

void PT2323::setMute_RL(char on) {
	this->setMute(MUTE_RL, on);
}

void PT2323::setMute_RR(char on) {
	this->setMute(MUTE_RR, on);
}

void PT2323::setMute_SW(char on) {
	this->setMute(MUTE_SW, on);
}

void PT2323::setMute_All(char on) {
	this->setMute(MUTE_ALL, on);
}*/