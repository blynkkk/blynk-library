# Implementing a Blynk HW client (library)

### Blynk library main functions

* Manage connection
* Serialize/deserialize Blynk protocol
* Handle direct pin operations
* Provide easy-to use API
 * Virtual pin handlers registration
 * Provide comfortable wrappers for some widgets

## Blynk protocol

Basic protocol description can be found [here](https://github.com/blynkkk/blynk-server/blob/master/README_FOR_APP_DEVS.md#protocol-messages).

Typical Blynk library knows how to send(S)/process(P):

    S   BLYNK_CMD_LOGIN + auth token
    SP  BLYNK_CMD_PING
    SP  BLYNK_CMD_RESPONSE
    SP  BLYNK_CMD_BRIDGE
    SP  BLYNK_CMD_HARDWARE
    S   BLYNK_CMD_TWEET
    S   BLYNK_CMD_EMAIL
    S   BLYNK_CMD_PUSH_NOTIFICATION

## HARDWARE/BRIDGE command body

The message body of these commands are encoded as a sequence of strings, separated by '\0' ([Null character](http://en.wikipedia.org/wiki/Null_character)).
Please note that the last value may be not Null-terminated.
In the following command examples \0 chars are replaced with spaces.

### Pin mode

PinMode command is received by library after connection, or when a mobile application starts.

    pm <pin> <mode>
    pm <pin> <mode> <pin> <mode> <pin> <mode> ...

Mode:
* in - INPUT
* out - OUTPUT
* pu - INPUT_PULLUP
* pd - INPUT_PULLDOWN

### Digital pin operations

Digital write:

    dw <pin> <val>

Digital read:

    dr <pin>

### Analog pin operations

    aw <pin> <val>

    ar <pin>

### Virtual pin operations

    vw <pin> <param0> <param1> <param2> <param3> ...

    vr <pin>

### Other operations

    info

TODO

### Developer notes
* Values in HW commands are text-encoded.
* In response to dr/ar command, library should send dw/aw command on the same pin and with the same message id.
* These situations should cause a connection drop, or reconnection attempt:
 * Message with ID=0 is received
 * Message with unknown type is received

## Wrappers for widgets

### LCD

TODO

### SD card

TODO

### Example implementations

* [Pseudo-library in Python](https://github.com/blynkkk/blynk-library/blob/master/tests/pseudo-library.py) - use this to play with the protocol and understand the basics
* [Arduino](https://github.com/blynkkk/blynk-library)
* [Spark Core](https://github.com/vshymanskyy/blynk-library-spark)
