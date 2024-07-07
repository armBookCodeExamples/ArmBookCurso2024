#include "mbed.h"
#include "DHT.h"
#include "arm_book_lib.h"

int main()
{
    DigitalIn B1(BUTTON1);
    DHT sensor(D1, DHT::DHT11);
    DigitalOut Teyu(LED1);
    UnbufferedSerial pc(USBTX, USBRX, 9600);
    char buffer[10];

    while (true)
    {
        Teyu = !B1;
        delay(3);
        int err = sensor.read();
        if(err == DHT::SUCCESS)
        {
            sprintf(buffer, "T: %.2f\r\n", sensor.getTemperature(DHT::CELCIUS));
        } 
        else 
        {
            pc.write(buffer, 10);
        }
    }
}
