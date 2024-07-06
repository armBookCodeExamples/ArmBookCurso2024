#include "mbed.h"
#include "DHT.h"
#include "arm_book_lib.h"

int main()
{
    DHT dht(D8, DHT::DHT11);
    DigitalIn B1(BUTTON1);

    DigitalOut Teyu(LED1);

    while (true)
    {
        Teyu = !B1;
        delay(3);
        int err = dht.read();
        if (err == DHT::SUCCESS)
        {
            printf("T = %.1f°C\r\n", dht.getTemperature());
        } 
        else
        {
            printf("Error code : %d\r\n", err);
        }
        delay(3);
    }
}
