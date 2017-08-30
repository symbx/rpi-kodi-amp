//
// Created by symbx on 23.07.17.
//

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>
//#include <sys/stat.h>
#include "amplifier.h"

Amplifier::Amplifier() {
    this->_volume = 60;
    for (int i = 0; i < 6; ++i) {
        this->_trim[i] = 15;
    }
    for (int i = 0; i < 3; ++i) {
        this->_tone[i] = 7;
    }
    this->_input = 0;
    this->_functions = 0;
    for (int i = 0; i < 7; ++i) {
        this->_mutes[i] = 0;
    }
    this->_wire = new Wire(1);
    this->_processor = new PT2322(_wire);
    this->_selector = new PT2323(_wire);
}

Amplifier::~Amplifier() {
    delete this->_selector;
    delete this->_processor;
    delete this->_wire;
}

void Amplifier::run() {
    this->load();
    while(this->init()) {
	std::cout << "Wait for anplifier power on" << std::endl;
        usleep(5000000); //Wait until amplifier power on
    }
    this->_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    remove("/tmp/amplifier.sock");
    struct sockaddr_un saddr = {AF_UNIX, "/tmp/amplifier.sock"};
    bind(this->_sock, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(this->_sock, 4);
    //chmod("/tmp/amplifier.sock", 776);
    while(1) {
        int conn = accept(this->_sock, NULL, NULL);
        int out = 0;
        unsigned char *buff = new unsigned char[2]; //I think 2 is normal (1 for command, 1 for value)
        while(1) {
            ssize_t r = read(conn, buff, 2);
            if (r < 2) {
                close(conn);
                break;
            }
            out = this->process(conn, buff[0], buff[1]);
        }
        delete[] buff;
        if(out)
            break;
    }
    remove("/tmp/amplifier.sock");
}

void Amplifier::load() {
    char path[255];
    char *home = getenv("HOME");
    snprintf(path, sizeof(path), "%s/.config/amplifier", home);
    std::ifstream file(path);
    if(file.fail())
        return;
    file >> this->_volume;
    for (int i = 0; i < 6; ++i) {
        file >> this->_trim[i];
    }
    for (int i = 0; i < 3; ++i) {
        file >> this->_tone[i];
    }
    file.read(&this->_input, 1);
    this->_input -= 10;
    file.read(&this->_functions, 1);
    for (int i = 0; i < 7; ++i) {
        file >> this->_mutes[i];
    }
    file.close();
}

void Amplifier::save() {
    char path[255];
    char *home = getenv("HOME");
    snprintf(path, sizeof(path), "%s/.config/amplifier", home);
    std::ofstream file(path);
    file << this->_volume;
    for (int i = 0; i < 6; ++i) {
        file << this->_trim[i];
    }
    for (int i = 0; i < 3; ++i) {
        file << this->_tone[i];
    }
    this->_input += 10;
    file.write(&this->_input, 1);
    this->_input -= 10;
    file.write(&this->_functions, 1);
    for (int i = 0; i < 7; ++i) {
        file << this->_mutes[i];
    }
    file.flush();
    file.close();
}

int Amplifier::init() {
    usleep(300000); //300ms
    std::cout << "Check..." << std::endl;
    if(this->_processor->reset() < 0)
        return 1;
    std::cout << "Reset sent...may be it's online" << std::endl;
    this->_processor->inputSW();

    this->_processor->setMasterVolume(60 - this->_volume);
    if(this->_functions & 1)
        this->_processor->setFunctionTone(1);
    if(this->_functions & 2)
        this->_processor->setFunction3D(1);
    if(this->_functions & 4)
        this->_processor->setFunctionMute(1);
    if(this->_functions & 8)
        this->_selector->setSurround(1);
    if(this->_functions & 16)
        this->_selector->setMixChannel(1);

    for (char i = 0; i < 6; ++i) {
        this->_processor->setVolumeTrim((char) ((i + 1) << 4), this->_trim[i]);
    }

    this->_processor->setTone_Bass(this->_tone[0]);
    this->_processor->setTone_Middle(this->_tone[1]);
    this->_processor->setTone_Treble(this->_tone[2]);

    this->_selector->setInput(this->_input);
    return 0;
}

int Amplifier::process(int cli, unsigned char cmd, unsigned char val) {
    switch(cmd) {
        case MASTER_VOL_SET:
            this->_volume = this->fixVolume(val);
            this->_processor->setMasterVolume((char) (60 - this->_volume));
            this->save();
            return 0;
        case MASTER_VOL_GET:
            write(cli, &this->_volume, 1);
            return 0;
        case SET_VOL_TRIM:
            this->volTrim(val);
            return 0;
        case GET_VOL_TRIM:
            write(cli, &this->_trim[(val >> 4) - 1], 1);
            return 0;
        case SET_TONE:
            this->setTone(val);
            return 0;
        case GET_TONE:
            write(cli, &this->_tone[(val >> 4) - 9], 1);
            return 0;
        case F_TONE_DEFEAT:
            this->_processor->setFunctionTone(val);
            if(val)
                this->_functions |= 1;
            else
                this->_functions &= 0xFF - 1;
            this->save();
            return 0;
        case F_3D:
            this->_processor->setFunction3D(val);
            if(val)
                this->_functions |= 2;
            else
                this->_functions &= 0xFF - 2;
            this->save();
            return 0;
        case F_MUTE:
            this->_processor->setFunctionMute(val);
            if(val)
                this->_functions &= 0xFF - 4;
            else
                this->_functions |= 4;
            this->save();
            return 0;
        case INPUT:
            if(val >= 0x07 && val <= 0x0B) {
                this->_selector->setInput(val);
                this->_input = val;
                this->save();
            }
            return 0;
        case SURROUND:
            this->_selector->setSurround(val);
            if(val)
                this->_functions |= 8;
            else
                this->_functions &= 0xFF - 8;
            this->save();
            return 0;
        case MIX_CHANNELS:
            this->_selector->setMixChannel(val);
            if(val)
                this->_functions |= 16;
            else
                this->_functions &= 0xFF - 16;
            this->save();
            return 0;
        case MUTE:
            this->setMute(val);
            return 0;
        case A_INIT:
            this->init();
        case A_EXIT:
            if(val == 1)
                return 1;
            return 0;
        default:
            return 1;
    }
}

char Amplifier::fixVolume(char vol) {
    if(vol < 0)
        vol = 0;
    if(vol > 60)
        vol = 60;
    return vol;
}

void Amplifier::volTrim(char val) {
    char channel = (char) (val & 0xF0);
    char vol = (char) (val & 0x0F);
    if(channel >= 0x10 && channel <= 0x60/* && channel % 16 == 0*/) {
        this->_processor->setVolumeTrim(channel, vol);
        this->_trim[(channel >> 4) - 1] = vol;
        this->save();
    }
}

void Amplifier::setTone(char val) {
    char type = (char) (val & 0xF0);
    char tone = (char) (val & 0x0F);
    if(type >= 0x90 && type <= 0xB0) {
        this->_processor->setTone(type, tone);
        this->_tone[(type >> 4) - 9] = tone;
        this->save();
    }
}

void Amplifier::setMute(char val) {
    char channel = (char) ((val & 0xF0) >> 4);
    if(channel == 0x0C)
        channel = (char) 0xFE;
    char on = (char) (val & 0x01);
    if((channel >= 0x00 && channel <= 0x0A && channel % 2 == 0) || channel == 0xFE) {
        this->_selector->setMute(channel, on);
        if(channel == 0xFE)
            this->_mutes[6] = on;
        else
            this->_mutes[channel/2] = on;
        this->save();
    }
}
