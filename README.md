# README #

### Quickstart: Arduino + Ethernet shield ###

* In Arduino IDE select File -> Examples -> Blynk -> Basic -> SimpleEthernet
* Update auth token in the sketch and upload it to Arduino
* Connect your Arduino with Ethernet shield to the internet

### Troubleshooting ###

To enable debug prints on default Serial, add on the top of your sketch (should be the first line):

        #define BLYNK_PRINT Serial

And enable serial in setup():

        Serial.begin(115200);

Alternatively, you can uncomment a similar line in Blynk/BlynkConfig.h, which would enable more prints.

This will not work if the same serial is used for Blynk connection (like SimpleSerial example).

### Command examples ###

#### Pin Mode ####
Application should setup pins once before using them, for example when dashboard starts.

+ Set pin mode to output:

        hardware pm 9 out

+ Set pin mode to input:

        hardware pm 9 in

+ Set pin mode to input pull-up:

        hardware pm 9 pu

+ Set pin mode to input pull-down (no available on Arduino):

        hardware pm 9 pd

+ Combined:

        hardware pm 1 in 13 out 9 out 8 in

#### Digital ####

+ Digital write:

        hardware dw 9 1
        hardware dw 9 0

+ Digital read:

        hardware dr 9
        You should receive response: dw 9 <val>

#### Analog ####

+ Analog write:

        hardware aw 14 123

+ Analog read:

        hardware ar 14
        You should receive response: aw 14 <val>

#### Virtual ####

+ Virtual write (virt. pin 9):

        hardware vw 9 1234
        hardware vw 9 string
        hardware vw 9 item1 item2 item3
        hardware vw 9 key1 val1 key2 val2

+ Virtual read:

        hardware vr 9
        You should receive response: vw 9 <values>

+ Info:

        hardware info

You should receive:

* Library version
* Input buffer size (maximum incoming message)
* Keep-alive timeout
* Device type (Arduino, Linux, ESP8266 ...)
* CPU type (ATmega328P, ATmega32U4 ...)