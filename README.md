# README #

### Quickstart: Arduino + Ethernet shield ###

* Download Blynk app from the market (iOS & Android).
* Get auth token from the app
* Import this library to Arduino IDE. Guide [here](http://arduino.cc/en/guide/libraries).
* In Arduino IDE, select File -> Examples -> Blynk -> Transports -> Ethernet
* Update auth token in the sketch and upload it to Arduino
* Connect your Arduino with Ethernet shield to the internet

### Troubleshooting ###

To enable debug prints on default Serial, add on the top of your sketch (should be the first line):

        #define BLYNK_DEBUG // Optional, this enables lots of prints
        #define BLYNK_PRINT Serial

And enable serial in setup():

        Serial.begin(9600);

### License ###

This project is released under The MIT License (MIT)
