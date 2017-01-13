[![GitHub version](https://img.shields.io/github/release/blynkkk/blynk-library.svg)](https://github.com/blynkkk/blynk-library/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/blynkkk/blynk-library/total.svg)](https://github.com/blynkkk/blynk-library/releases/latest)
[![GitHub stars](https://img.shields.io/github/stars/blynkkk/blynk-library.svg)](https://github.com/blynkkk/blynk-library/stargazers)
[![GitHub issues](https://img.shields.io/github/issues/blynkkk/blynk-library.svg)](https://github.com/blynkkk/blynk-library/issues)
[![Build Status](https://img.shields.io/travis/blynkkk/blynk-library.svg)](https://travis-ci.org/blynkkk/blynk-library)
[![GitHub wiki](https://img.shields.io/badge/Wiki-available-brightgreen.svg)](https://github.com/blynkkk/blynk-library/wiki)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/blynkkk/blynk-library/blob/master/LICENSE)

If you like **Blynk** - give it a star, or fork it and contribute! 
[![GitHub stars](https://img.shields.io/github/stars/blynkkk/blynk-library.svg?style=social&label=Star)](https://github.com/blynkkk/blynk-library/stargazers) 
[![GitHub forks](https://img.shields.io/github/forks/blynkkk/blynk-library.svg?style=social&label=Fork)](https://github.com/blynkkk/blynk-library/network)

## Download
https://github.com/blynkkk/blynk-library/releases/latest

## Documentation
Blynk Docs: http://docs.blynk.cc/#blynk-firmware  
Wiki: https://github.com/blynkkk/blynk-library/wiki

__________

# What is Blynk?
Blynk is a platform with iOS and Android apps to control Arduino, Raspberry Pi and the likes over the Internet.
You can easily build graphic interfaces for all your projects by simply dragging and dropping widgets.
If you need more information, please follow these links:

* Social: [Webpage](http://www.blynk.cc) / [Facebook](http://www.fb.com/blynkapp) / [Twitter](http://twitter.com/blynk_app) / [Kickstarter](https://www.kickstarter.com/projects/167134865/blynk-build-an-app-for-your-arduino-project-in-5-m/description)
* Join our [Blynk community forum](http://community.blynk.cc)
* Smartphone Apps: [Google Play](https://play.google.com/store/apps/details?id=cc.blynk) / [App Store](https://itunes.apple.com/us/app/blynk-control-arduino-raspberry/id808760481?ls=1&mt=8)
* Deploy your own [Blynk Server](https://github.com/blynkkk/blynk-server)

![Dashboard settings](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/dash_settings.png)
![Widgets Box](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/widgets_box.png)
![Button edit](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/button_edit.png)
![Dashboard](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/dash.png)

### Quickstart: Arduino + Ethernet shield

* Download the Blynk app ([App Store](https://itunes.apple.com/us/app/blynk-control-arduino-raspberry/id808760481?ls=1&mt=8), [Google Play](https://play.google.com/store/apps/details?id=cc.blynk))
* Get the Auth Token from the app
* Import this library to Arduino IDE. Guide [here](http://arduino.cc/en/guide/libraries)
* In Arduino IDE, select File -> Examples -> Blynk -> Boards_Ethernet -> Arduino_Ethernet
* Update Auth Token in the sketch and upload it to Arduino
* Connect your Arduino with Ethernet shield to the internet

Please find examples on how to use different types of connections (transports) and how to do make something great with Blynk.
You can easily apply any type of board/connection to all examples.

### Supported boards, Ethernet, WiFi, Bluetooth, GSM, Serial, USB...

Full list of supported hardware is [here](https://github.com/blynkkk/blynkkk.github.io/blob/master/SupportedHardware.md).
Please [see examples](/examples) for different connection types.

__________

### Implementations for other platforms
* [Particle](https://github.com/vshymanskyy/blynk-library-spark)
* [Node.js + Espruino](https://github.com/vshymanskyy/blynk-library-js)
* [MicroPython WiPy](https://github.com/wipy/wipy/tree/master/lib/blynk)
* [OpenWrt](https://github.com/vshymanskyy/blynk-library-openwrt)
* [MBED](https://developer.mbed.org/users/vshymanskyy/code/Blynk/)
* [Node-RED](https://github.com/tzapu/node-red-contrib-blynk-websockets)
* [LabVIEW](https://github.com/juncaofish/NI-LabVIEWInterfaceforBlynk)

### License
This project is released under The MIT License (MIT)
