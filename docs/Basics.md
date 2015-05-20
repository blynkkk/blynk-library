# Basics

### Digital & Analog pins

### Virtual Pins

```cpp
Blynk.virtualWrite(pin, "abc");
Blynk.virtualWrite(pin, 123);
Blynk.virtualWrite(pin, 12.34);
```

This function will be called every time App Widget writes value to Virtual Pin 1:
```cpp
BLYNK_WRITE(1)
{
  BLYNK_LOG("Got a value: %s", param.asStr());
  // You can also use: param.asInt() and param.asDouble()
}
```

This function will be called every time App Widget requests data for Virtual Pin 5:
```cpp
BLYNK_READ(5)
{
  Blynk.virtualWrite(5, some_value);
}
```

### Tweet

```cpp
// Send a Tweet ;)
Blynk.tweet("some text");
```

### Push notification
This will display a short note on the screen of the smartphone:

```cpp
// Send a push notification to the App
Blynk.push_notification("some text");
```

### Email
Also you can send an email:

```cpp
// Send a pre-defined email message
Blynk.email();

// Send an email message with custom body
Blynk.email("some text");

// Send an email message with custom body, subject and recipient
Blynk.email("someone@blabla.com", "Subject", "some text");
```
P.S. Be sure to check the [troubleshooting page](./Troubleshooting.md)
