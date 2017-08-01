#include "amplifier.h"

int main() {
    Amplifier* amp = new Amplifier();
    amp->run();
    delete amp;
    return 0;
}