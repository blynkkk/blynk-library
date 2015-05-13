# Troubleshooting

### Connection

If you have connection problems, please try checking your wiring using the examples provided with your shields and hardware. Once you have some understanding how to configure connection, it will be much easier to use Blynk.

Also, try looking into [serial debug prints](./Troubleshooting.md#enable-debug).

### Delay

Your application might be calling a delay() function or sleeps/cycles for a long time inside of the loop(), like this:

    void loop()
    {
      ...
      delay(1000);
      other_long_operation();
      ...
      Blynk.run();
    }
    
You should be aware that this can degrade performance of Blynk, or cause connection drops.

If you need periodic actions, consider using some timer library, like shown [in this example](https://github.com/blynkkk/blynk-library/blob/master/examples/GettingStarted/PushData/PushData.ino).

### Flood

Your application may cause an enormous load on our server, please try avoiding sending data too fast.

For example, in this situation Blynk.run() will quickly finish processing incoming messages, and then new value is sent to the server immediately, causing high traffic:

    void loop()
    {
      Blynk.virtualWrite(1, value);
      Blynk.run();
    }

You might be thinking about adding a delay(), but this creates [a different trouble](./Troubleshooting.md#delay).

If you need periodic actions, consider using some timer library, like shown [in this example](https://github.com/blynkkk/blynk-library/blob/master/examples/GettingStarted/PushData/PushData.ino).

### Enable debug

To enable debug prints on the default Serial, add on the top of your sketch **(should be the first line
)**:

        #define BLYNK_DEBUG // Optional, this enables lots of prints
        #define BLYNK_PRINT Serial

And enable serial in setup():

        Serial.begin(9600);
        
You can also use spare Hardware serial ports or SoftwareSerial for debug output (you will need an adapter to connect to it with your PC).

**WARNING : enabling debug mode will slowdown your hardware processing speed up to 10 times**.
