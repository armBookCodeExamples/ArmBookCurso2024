#include "mbed.h"

int main()
{
    DigitalIn B1(BUTTON1);

    DigitalOut Teyu(LED1);

    while (true) {
        Teyu = !B1;
    }
}
