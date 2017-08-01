#include "../lib/wire.h"

#ifndef PT2323_H
#define PT2323_H

class PT2323
{
	public:
		PT2323(Wire* wire);
		void writeByte(char b);
		/*void initialize();*/
		void setInput(char line);
		/*void setInput_L1();
		void setInput_L2();
		void setInput_L3();
		void setInput_L4();
		void setInput_C6();*/
		void setSurround(char on);
		void setMixChannel(char on);
		void setMute(char chan, char on);
		/*void setMute_FL(char on);
		void setMute_FR(char on);
		void setMute_CT(char on);
		void setMute_RL(char on);
		void setMute_RR(char on);
		void setMute_SW(char on);
		void setMute_All(char on);*/

		const char ADDRESS     = 0x4A; /* 0x94 >> 1 */
		const char INPUT_C6    = 0x07;
		const char INPUT_L1    = 0x08;
		const char INPUT_L2    = 0x09;
		const char INPUT_L3    = 0x0A;
		const char INPUT_L4    = 0x0B;
		const char MIX_CHANNEL = 0x90;
		const char SET_INPUT   = 0xC0;
		const char SURROUND    = 0xD0;
		const char MUTE        = 0xF0;
		const char MUTE_FL     = 0x00;
		const char MUTE_FR     = 0x02;
		const char MUTE_CT     = 0x04;
		const char MUTE_SW     = 0x06;
		const char MUTE_RL     = 0x08;
		const char MUTE_RR     = 0x0A;
		const char MUTE_ALL    = 0xFE;

	protected:
		Wire* _wire;
};

#endif