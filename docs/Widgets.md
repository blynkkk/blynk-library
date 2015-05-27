# Widgets

### Bridge

Bridge can be used for Device-to-Device communication.
You can send digital/analog/virtual write commands from one device to another, knowing it's auth token.

```cpp
// Bridge widget on virtual pin 1
WidgetBridge bridge1(1);
...
bridge1.setAuthToken("OtherAuthToken");
...
bridge1.digitalWrite(9, HIGH);
bridge1.analogWrite(10, 123);
bridge1.virtualWrite(1, "hello");
```
