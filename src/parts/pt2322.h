#include "../lib/wire.h"

#ifndef PT2322_H
#define PT2322_H

class PT2322
{
	public:
		PT2322(Wire* wire);
		int writeByte(char b);
		/*void initialize();*/
		int reset();
		void setMasterVolume(char vol);
		void setVolumeTrim(char chan, char trim);
		/*void setVolumeTrim_FL(char trim);
		void setVolumeTrim_FR(char trim);
		void setVolumeTrim_CT(char trim);
		void setVolumeTrim_RL(char trim);
		void setVolumeTrim_RR(char trim);
		void setVolumeTrim_SW(char trim);*/
		void setTone(char type, char tone);
		void setTone_Bass(char tone);
		void setTone_Middle(char tone);
		void setTone_Treble(char tone);
		void inputSW();
		void setFunctionTone(char on);
		void setFunction3D(char on);
		void setFunctionMute(char on);

		const char ADDRESS     = 0x44; /* 0x88 >> 1 */
		const char FUNC_TONE   = 0x02;
		const char FUNC_3D     = 0x04;
		const char FUNC_MUTE   = 0x08;
		/*const char VOL_TRIM_FL = 0x10;
		const char VOL_TRIM_FR = 0x20;
		const char VOL_TRIM_CT = 0x30;
		const char VOL_TRIM_RL = 0x40;
		const char VOL_TRIM_RR = 0x50;
		const char VOL_TRIM_SW = 0x60;*/
		const char FUNCTION    = 0x70;
		const char TONE_BASS   = 0x90;
		const char TONE_MIDDLE = 0xA0;
		const char TONE_TREBLE = 0xB0;
		const char INPUT_SW    = 0xC7;
		const char VOL_1STEP   = 0xD0;
		const char VOL_10STEP  = 0xE0;
		const char SYS_RESET   = 0xFF;
	
	protected:
		Wire* _wire;
		char _functions;
};
#endif