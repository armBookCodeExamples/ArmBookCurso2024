//=====[Libraries]=============================================================

#include "control_system.h"

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    controlSystemInit();
    while (true) {
        controlSystemUpdate();
    }
}