# What is Blynk?
Blynk is a platform with iOS and Android apps to control Arduino, Raspberry Pi and the likes over the Internet.  
You can easily build graphic interfaces for all your projects by simply dragging and dropping widgets.
If you need more information, please follow these links:
* [Kickstarter](https://www.kickstarter.com/projects/167134865/blynk-build-an-app-for-your-arduino-project-in-5-m/description).
* [Blynk downloads, docs, tutorials](http://www.blynk.cc)
* [Blynk community](http://community.blynk.cc)
* [Facebook](http://www.fb.com/blynkapp)
* [Twitter](http://twitter.com/blynk_app)

Please find examples on how to use different types of connections (transports) and how to do make something great with Blynk.
You can easily apply any type of board/connection to all examples.

### Quickstart: Arduino + Ethernet shield ###

* Download Blynk app (soon in the App Store and Google Play) 
* Get the Auth Token from the app
* Import this library to Arduino IDE. Guide [here](http://arduino.cc/en/guide/libraries)
* In Arduino IDE, select File -> Examples -> Blynk -> Transports -> Ethernet
* Update Auth Token in the sketch and upload it to Arduino
* Connect your Arduino with Ethernet shield to the internet

### Supported hardware ###

[The list of supported hardware](http://community.blynk.cc/t/hardware-supported-by-blynk)

### Troubleshooting ###

To enable debug prints on default Serial, add on the top of your sketch.

**Should be the first line**

**WARNING : enabling debug mode will slowdown your hardware processing speed up to 10 times**:

        #define BLYNK_DEBUG // Optional, this enables lots of prints
        #define BLYNK_PRINT Serial

And enable serial in setup():

        Serial.begin(9600);

### License ###

This project is released under The MIT License (MIT)
