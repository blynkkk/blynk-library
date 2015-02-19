# README #

### Quickstart: Arduino + Ethernet shield ###

* In Arduino IDE select File -> Examples -> Blynk -> Basic -> SimpleEthernet
* Update auth token in the sketch and upload it to Arduino
* Connect your Arduino with Ethernet shield to the internet

### Troubleshooting ###

To enable debug prints on default Serial (baud rate 9600), add BLYNK_DEBUG as a first line in your code:

        #define BLYNK_DEBUG

Alternatively, you can uncomment a similar line in Blynk/BlynkConfig.h, which will enable more prints.

This won't' work if the same serial is used for Blynk connection (like SimpleSerial example).

### Command examples ###

#### Pin Mode ####
Application should setup pin modes(input or output) before using them, when dashboard is switched from Edit mode to Run mode.
Pin Mode is affected by the Widget type. 
+ All the pins for Widgets that perform control functions (e.g Button, Slider, Joystick, etc.) should work in Output Mode 
+ All the pins for Widgets that performs data vizualization (e.g LCD display, Graph, Numerical Display, etc.) should work in Input Mode   

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

### Login data ###
During connection, library provides info about device:

* Auth token
* Device type (Arduino, Linux, ESP8266 ...)
* Input buffer size (maximum incoming message)
* Keep-alive timeout