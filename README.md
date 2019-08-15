# Blynk C++ Library [![Tweet](https://img.shields.io/twitter/url/http/shields.io.svg?style=social)](https://twitter.com/intent/tweet?text=Build%20your%20IoT%20App%20in%20minutes,%20right%20on%20your%20smartphone!&url=https://github.com/blynkkk/blynk-library&via=blynk_app&hashtags=IoT,iOS,Android,Arduino,ESP8266,ESP32,RaspberryPi)

[![GitHub version](https://img.shields.io/github/release/blynkkk/blynk-library.svg)](https://github.com/blynkkk/blynk-library/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/blynkkk/blynk-library/total.svg)](https://github.com/blynkkk/blynk-library/releases/latest)
[![GitHub stars](https://img.shields.io/github/stars/blynkkk/blynk-library.svg)](https://github.com/blynkkk/blynk-library/stargazers)
[![GitHub issues](https://img.shields.io/github/issues/blynkkk/blynk-library.svg)](https://github.com/blynkkk/blynk-library/issues)
[![Build Status](https://img.shields.io/travis/blynkkk/blynk-library.svg)](https://travis-ci.org/blynkkk/blynk-library)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/blynkkk/blynk-library/blob/master/LICENSE)

If you like **Blynk** - give it a star, or fork it and contribute! 
[![GitHub stars](https://img.shields.io/github/stars/blynkkk/blynk-library.svg?style=social&label=Star)](https://github.com/blynkkk/blynk-library/stargazers) 
[![GitHub forks](https://img.shields.io/github/forks/blynkkk/blynk-library.svg?style=social&label=Fork)](https://github.com/blynkkk/blynk-library/network)
__________

### Blynk is **the most popular Internet of Things platform** for connecting any hardware to the cloud, designing apps to control them, and managing your deployed products at scale. 

- With Blynk Library you can connect **over 400 hardware models** (including ESP8266, ESP32, NodeMCU, all Arduinos, Raspberry Pi, Particle, Texas Instruments, etc.)to the Blynk Cloud.
Full list of supported hardware can be found [here](https://github.com/blynkkk/blynkkk.github.io/blob/master/SupportedHardware.md).

- With Blynk apps for **iOS** and **Android** apps you can easily drag-n-drop graphic interfaces for any DIY or commercial project. It's a pure WYSIWG experience: no coding on iOS or Android required. 

- Hardware can connect to Blynk Cloud (open-source server) over the Internet using hardware connectivity available on your board (like ESP32), or with the use of various shields (Ethernet, WiFi, GSM, LTE, etc). Blynk Cloud is available for every user of Blynk **for free**. Direct connection over Bluetooth is also possible. 

![Blynk Banner](https://github.com/blynkkk/blynkkk.github.io/blob/master/images/GithubBanner.jpg)

## Downloads

**Blynk [Arduino Library](https://github.com/blynkkk/blynk-library/releases/latest)**

**Blynk Mobile App: 
[<img src="https://cdn.rawgit.com/simple-icons/simple-icons/develop/icons/googleplay.svg" width="18" height="18" /> Google Play](https://play.google.com/store/apps/details?id=cc.blynk) | 
[<img src="https://cdn.rawgit.com/simple-icons/simple-icons/develop/icons/apple.svg" width="18" height="18" /> App Store](https://itunes.apple.com/us/app/blynk-control-arduino-raspberry/id808760481?ls=1&mt=8)**

Optionally you can install **Blynk [Local Server](https://github.com/blynkkk/blynk-server)** and run everything locally. However, **Blynk Cloud is free** for anyone who is using Blynk for personal (non-commercial) use.


## Quickstart: Arduino + Ethernet shield

* Download the Blynk app ([App Store](https://itunes.apple.com/us/app/blynk-control-arduino-raspberry/id808760481?ls=1&mt=8), [Google Play](https://play.google.com/store/apps/details?id=cc.blynk))
* Get the Auth Token from the app
* Import this library to Arduino IDE. Guide [here](http://arduino.cc/en/guide/libraries)
* In Arduino IDE, select File -> Examples -> Blynk -> Boards_Ethernet -> Arduino_Ethernet
* Update Auth Token in the sketch and upload it to Arduino
* Connect your Arduino with Ethernet shield to the internet

When you are connected - check the included examples on how to use different types of connections (transports) and explore Blynk features. You can combine any example for your hardware + transport + features.

## Supported boards, Ethernet, WiFi, Bluetooth, GSM, Serial, USB...

Full list of supported hardware is [here](https://github.com/blynkkk/blynkkk.github.io/blob/master/SupportedHardware.md).  
Check out our Examples Browser: http://examples.blynk.cc

## Documentation and other helpful links

[Full Blynk Documentation](http://docs.blynk.cc/#blynk-firmware) - a complete guide on Blynk features  
[Community (Forum)](http://community.blynk.cc) - join a 500,000 Blynk community to ask questions and share ideas  
[Help Center](http://help.blynk.cc) - helpful articles on various Blynk aspects  
[Code Examples Browser](http://examples.blynk.cc) - browse examples to explore Blynk possibilities  
[Official Website](https://blynk.io)

**Social Media:**

[Facebook](https://www.fb.com/blynkapp) | 
[Twitter](https://twitter.com/blynk_app) | 
[Youtube](https://www.youtube.com/blynk) | 
[Instagram](https://www.instagram.com/blynk.iot/) | 
[LinkedIn](https://www.linkedin.com/company/b-l-y-n-k/)

## Blynk libraries for other platforms
* [Python, MicroPython](https://github.com/blynkkk/lib-python)

## Libraries by community
* [Node.js, Espruino, Browsers](https://github.com/vshymanskyy/blynk-library-js)
* [Python, MicroPython](https://github.com/vshymanskyy/blynk-library-python)
* [Particle](https://github.com/vshymanskyy/blynk-library-spark)
* [Lua, OpenWrt, NodeMCU](https://github.com/vshymanskyy/blynk-library-lua)
* [OpenWrt packages](https://github.com/vshymanskyy/blynk-library-openwrt)
* [MBED](https://developer.mbed.org/users/vshymanskyy/code/Blynk/)
* [Node-RED](https://www.npmjs.com/package/node-red-contrib-blynk-ws)
* [LabVIEW](https://github.com/juncaofish/NI-LabVIEWInterfaceforBlynk)
* [C#](https://github.com/sverrefroy/BlynkLibrary)

## Contributing
We accept contributions from our community: stability bugfixes, new hardware support, or any other improvements.  
[Here](https://github.com/blynkkk/blynk-library/labels/help%20wanted) is a list of what you could help with.

### License
This project is released under The MIT License (MIT)
