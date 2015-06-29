# Basics

## Features of Blynk
* Similar API & UI for all supported hardware & devices
* Connection to the cloud using:
  * Ethernet
  * WiFi
  * USB (Serial)
  * ...
* Set of easy-to-use widgets
* Direct pin manipulation with no code writing
* Easy to integrate and add new functionality using virtual pins
* Device-to-Device communication using Bridge
* Sending emails, tweets etc.
* ... there will be more!

### Digital & Analog pins

The library can perform basic pin IO (input-output) operations out-of-the-box:

    digitalRead
    digitalWrite
    analogRead
    analogWrite (PWM or Analog signal depending on the platform)

So, there is no need to write code for simple things like LED, Relay control, and analog sensors.

### Virtual Pins

Virtual pins are used to interface with libraries (Servo, LCD and others), and implement custom functionality.  
The device may send data to the widget on a virtual pin like this:

```cpp
Blynk.virtualWrite(pin, "abc");
Blynk.virtualWrite(pin, 123);
Blynk.virtualWrite(pin, 12.34);
```

Also, virtual pins can react to value updates and requests.

For example, this function will be called every time App Widget requests data for Virtual Pin 5:
```cpp
BLYNK_READ(5)
{
  // Usually, you will need to respond with a virtual pin value.
  Blynk.virtualWrite(5, some_value);
}
```

This function will be called every time App Widget writes value to Virtual Pin 1:
```cpp
BLYNK_WRITE(1)
{
  BLYNK_LOG("Got a value: %s", param.asStr());
  // You can also use: param.asInt() and param.asDouble()
}
```

BLYNK_READ/BLYNK_WRITE functions are effectively "getters/setters" of the Virtual Pins if you're familiar with this concept in other programming languages.  
Please also take into account that these functions should take minimal time to execute, so avoid using sleep/delay inside of them.

### Voluntary / Requested data updates

The device may send data to the cloud at any time, but we provide 2 common ways of doing it:
* "Pushing"  
  In this case the refresh rate is set to "PUSH" in the App Widget settings. Then device decides when to send new data to the App. Remember not to send the data too fast!
* "Requested"  
  In this case the refresh rate is selected in the App Widget settings, and BLYNK_READ handler needs to be implemented. **This works only for Virtual Pins for now.**

### Data format

The actual values are sent as strings, so there is no practical limits on the data that can be sent.  
However, remember the limitations of the platform when dealing with numbers.  
For example the integer on Arduino is 16-bit, allowing range -32768 to 32767.

Actually, the data format is an array of strings, so these are also valid:

```cpp
param[0].asInt()
param[1].asDouble()
```

This is needed for more complex widgets.
You can also get the RAW data from the param buffer:

```cpp
param.getBuffer()
param.getLength()
```

### Tweet

For tweeting, you need to configure the tweet widget in the App.

```cpp
// Send a Tweet ;)
Blynk.tweet("some text");
```

### Push notification

This will display a short note on the screen of the smartphone:

```cpp
// Send a push notification to the App
Blynk.notify("some text");
```

### Email

Also you can send an email:

```cpp
// Send an email message with custom body, subject and recipient
Blynk.email("someone@blabla.com", "Subject", "some text");
```

### P.S.

Be sure to check the:
* [Widgets](./Widgets.md)
* [Platforms](./Platforms.md)
* [Troubleshooting](./Troubleshooting.md)
