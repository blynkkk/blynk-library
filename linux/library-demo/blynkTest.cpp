// Simple test for the blynk library on the Raspberry.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <BlynkSocket.h>
#include <wiringPi.h>
#include <BlynkApiWiringPi.h>

#define  AUTH "your-api-key" // add your API key here

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);

int main(int argc, char* argv[])
{
    unsigned int val = 0;
    static unsigned int sec=0;

    srand(time(NULL));
    Blynk.begin( AUTH );


    while( 1 )
    {

        Blynk.run(); // Blynk housekeeping
        sleep (1); // 1 sec
        if( ++sec >= 60)
        {
            sec = 0;
            val = (rand() % 500); // 0..499
            printf( "Random value: %d\n", val );
            Blynk.virtualWrite(V01, val );   // Send value to Blynk's V01
        } // if <minute passed>

    }
}
