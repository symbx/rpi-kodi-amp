//
// Created by symbx on 23.07.17.
//

#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include "lib/wire.h"
#include "parts/pt2322.h"
#include "parts/pt2323.h"

#ifndef AMPLIFIER_AMPLIFIER_H
#define AMPLIFIER_AMPLIFIER_H


class Amplifier {
    public:
        Amplifier();
        ~Amplifier();
        void run();
        enum Command {
            /* processor */
            MASTER_VOL_SET = 1,
            MASTER_VOL_GET = 2,
            SET_VOL_TRIM = 3,
            GET_VOL_TRIM = 4,
            SET_TONE = 5,
            GET_TONE = 6,
            F_TONE_DEFEAT = 7,
            F_3D = 8,
            F_MUTE = 9,
            /* selector */
            INPUT = 10,
            SURROUND = 11,
            MIX_CHANNELS = 12,
            MUTE = 13,
            /* app */
            A_INIT = 127,
            A_EXIT = 128
        };

    protected:
        int _sock;
        char _volume;
        char _trim[6];
        char _tone[3];
        char _functions;
        char _input;
        char _mutes[7];
        Wire* _wire;
        PT2322* _processor;
        PT2323* _selector;
        void load();
        void save();
        int init();
        int process(int cli, unsigned char cmd, unsigned char val);
        char fixVolume(char vol);
        void volTrim(char val);
        void setTone(char tone);
        void setMute(char val);
};


#endif //AMPLIFIER_AMPLIFIER_H
