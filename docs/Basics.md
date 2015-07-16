# Basics

## Features of Blynk
* Similar API & UI for all supported hardware & devices
* Connection to the cloud using:
  * Ethernet
  * WiFi
  * USB (Serial)
  * ...
* Set of easy-to-use Widgets
* Direct pin manipulation with no code writing
* Easy to integrate and add new functionality using virtual pins
* Device-to-Device communication using Bridge Widget
* Sending emails, tweets etc.
* ... more features are constantly added!

### Configuration

The simplest way to configure Blynk is to call Blynk.begin():
```cpp
Blynk.begin(auth, ...);
```
begin() can have different parameters for different boards, so follow the example for your board/type of connection.

A more advanced way is to setup the shield (WiFi, Ethernet) manually, and then call ``` blynk.config(...)```:

```cpp
Blynk.config(auth);
```
```cpp
Blynk.config(auth, server, port);
```

For WiFi connections, you can use a ``` connectWiFi ``` (just for convenience).
```cpp
Blynk.connectWiFi(ssid, pass)
```
To connect to Open WiFi networks, set pass to an empty string ("").

### Connection management

There are several functions to help with connection management:
```cpp
# This will try connecting to Blynk server. Default timeout is 30 seconds
bool result = Blynk.connect();
bool result = Blynk.connect(timeout);
```
```cpp
# This will disconnect from Blynk server
Blynk.disconnect();
```
```cpp
# This returns if we're currently connected
bool result = Blynk.connected();
```

**Note:** Just after ``` Blynk.begin(...) ``` or ``` Blynk.config(...) ```, Blynk is not yet connected to the server.
It will try to connect when it hits first ``` Blynk.run() ``` or ``` Blynk.connect() ``` call.

If you want to skip connecting to the server, just call ``` disconnect() ``` right after configuration.

If your shield/connection type is not supported yet - you can craft it yourself easily! [Here is an example](https://github.com/blynkkk/blynk-library/blob/master/examples/BoardsAndShields/User_Defined_Connection/User_Defined_Connection.ino).

### Blynk.run()
This function should be called frequently to process incoming commands and perform housekeeping of Blynk connection.
It is usually called in ``` void loop() {} ```.
You can call it in other places, unless you run out of heap memory (in the cascaded functions with local memory).
For example, it is not recommended to call ``` Blynk.run() ``` inside of the  ```BLYNK_READ ``` and ``` BLYNK_WRITE ``` functions on low-RAM devices.

### Digital & Analog pins

The library can perform basic pin IO (input-output) operations out-of-the-box:

    digitalRead
    digitalWrite
    analogRead
    analogWrite (PWM or Analog signal depending on the platform)

So, there is no need to write code for simple things like LED, Relay control, and analog sensors.

### Virtual Pins

Virtual Pins are designed to send any data from your microcontroller to the Blynk App and back. 
Think about Virtual Pins as channels for sending any variables. Make sure you differentiate Virtual Pins from physical pins on your hardware. Virtual Pins have no physical representation. 

Virtual Pins can be used to interface with libraries (Servo, LCD and others), and implement custom functionality.  
The device may send data to the Widget to the Virtual Pin like this:

```cpp
Blynk.virtualWrite(pin, "abc");
Blynk.virtualWrite(pin, 123);
Blynk.virtualWrite(pin, 12.34);
```

Also, Virtual Pins can react to value updates and requests.

For example, this function will be called every time Widget in the Blynk App requests data for Virtual Pin 5:
```cpp
BLYNK_READ(5)
{
  // Usually, you will need to respond with a virtual pin value.
  Blynk.virtualWrite(5, some_value);
}
```

This function will be called every time Widget in the Blynk App writes value to Virtual Pin 1:
```cpp
BLYNK_WRITE(1)
{
  BLYNK_LOG("Got a value: %s", param.asStr());
  // You can also use: param.asInt() and param.asDouble()
}
```

``` BLYNK_READ/BLYNK_WRITE ``` functions are effectively "getters/setters" of the Virtual Pins if you're familiar with this concept in other programming languages.  
Please also take into account that these functions should take minimal time to execute, so **avoid using sleep/delay** inside of them.

### Voluntary / Requested data updates

Devices can send data to the cloud at any time, but we provide 2 common ways of doing it:
* "Pushing"  
  In this case the refresh rate should be set to "PUSH" in Blynk App: Widget Settings -> Frequency -> PUSH. Then device decides when to send new data to the App. Remember not to send the data too fast!
* "Requested"  
  In this case the refresh frequency is selected in Blynk App: Widget Settings -> Frequency, and BLYNK_READ handler needs to be implemented. **This works only for Virtual Pins for now.**

### Data format

The actual values are sent as strings, so there is no practical limits on the data that can be sent.  
However, remember the limitations of the platform when dealing with numbers.  
For example the integer on Arduino is 16-bit, allowing range -32768 to 32767.

The data format is an array of strings, so these are also valid:

```cpp
param[0].asInt()
param[1].asDouble()
```

It can be used in more complex Widgets, which send an array of values. For example, Joystick Widget can write 2 values into one Virtual Pin:

```cpp
x = param[0].asInt();
y = param[1].asInt();
```


You can also get the RAW data from the param buffer:

```cpp
param.getBuffer()
param.getLength()
```

### Tweet

For tweeting, you need to add and configure Twitter Widget in the Blynk App. Project should be active: press ►.

```cpp
// Send a Tweet ;)
Blynk.tweet("some text");
```


### Push notification

This will send a native Push Notification to smartphone:

```cpp
// Send a push notification to the App
Blynk.notify("some text");
```
Push Notification Widget should be installed in the project in Blynk App. Project should be active: press ►.

### Email

Also you can send an email:

```cpp
// Send an email message with custom body, subject and recipient
Blynk.email("someone@example.com", "Subject", "Email body");
```
Email Widget should be installed in the project in Blynk App. Project should be active: press ►.


### P.S.

Be sure to check the:
* [Widgets](./Widgets.md)
* [Platforms](./Platforms.md)
* [Troubleshooting](./Troubleshooting.md)
