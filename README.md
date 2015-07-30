[![Build Status](https://travis-ci.org/blynkkk/blynk-library.svg)](https://travis-ci.org/blynkkk/blynk-library)

Implementations for other platforms:
* [Node.js + Espruino](https://github.com/vshymanskyy/blynk-library-js)
* [Particle Core](https://github.com/vshymanskyy/blynk-library-spark)

__________

# What is Blynk?
Blynk is a platform with iOS and Android apps to control Arduino, Raspberry Pi and the likes over the Internet.
You can easily build graphic interfaces for all your projects by simply dragging and dropping widgets.
If you need more information, please follow these links:

* [Kickstarter campaign](https://www.kickstarter.com/projects/167134865/blynk-build-an-app-for-your-arduino-project-in-5-m/description)
* [Blynk downloads, docs, tutorials](http://www.blynk.cc)
* [Blynk community](http://community.blynk.cc)
* [Facebook](http://www.fb.com/blynkapp)
* [Twitter](http://twitter.com/blynk_app)

Please find examples on how to use different types of connections (transports) and how to do make something great with Blynk.
You can easily apply any type of board/connection to all examples.

### Quickstart: Arduino + Ethernet shield

* Download the Blynk app ([App Store](https://itunes.apple.com/us/app/blynk-control-arduino-raspberry/id808760481?ls=1&mt=8), [Google Play](https://play.google.com/store/apps/details?id=cc.blynk))
* Get the Auth Token from the app
* Import this library to Arduino IDE. Guide [here](http://arduino.cc/en/guide/libraries)
* In Arduino IDE, select File -> Examples -> Blynk -> BoardsAndShields -> Arduino_Ethernet
* Update Auth Token in the sketch and upload it to Arduino
* Connect your Arduino with Ethernet shield to the internet

### Supported boards, WiFi, Serial, USB...

[See examples](examples/BoardsAndShields) with different connection types.

Full list of supported hardware is [here](http://community.blynk.cc/t/hardware-supported-by-blynk).

### Docs

* [Basics](./docs/Basics.md)
* [Widgets](./docs/Widgets.md)
* [Security](./docs/Security.md)
* [Platforms/Connection types](./docs/Platforms.md)
* [Troubleshooting](./docs/Troubleshooting.md)
* [Implementing your own library](./docs/Implementing.md)

### License

This project is released under The MIT License (MIT)
