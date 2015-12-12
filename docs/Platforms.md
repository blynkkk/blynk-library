# Platforms


## Arduino

```cpp
TODO
```

## Particle (formely Spark Core)

```cpp
TODO
```

## ESP8266 (standalone)

You can run Blynk directly on the ESP8266!

Install the latest ESP8266 for Arduino using this guide:
https://github.com/esp8266/Arduino#installing-with-boards-manager13

**Example:** [ESP8266_Standalone](https://github.com/blynkkk/blynk-library/blob/master/examples/BoardsAndShields/ESP8266_Standalone/ESP8266_Standalone.ino)

## Linux (Raspberry Pi)


###One-Line-Install

Open a commandline and paste:
```bash
curl https://gist.githubusercontent.com/csicar/c2d9e492159ddce56f34/raw/5acdf89cb7f998790c159c1bc86c932e2838257a/blynk-install.sh | sh
```
Blynk will be installed with all dependencies and added to the autorun on linux.

###Manual

0. Connect your Raspberry Pi to the internet and open it's console. ^_^

1. Install WiringPi:
    http://wiringpi.com/download-and-install/

2. Download and build Blynk:
    ```bash
    $ git clone https://github.com/blynkkk/blynk-library.git
    $ cd blynk-library/linux
    $ make clean all target=raspberry
    ```

3. Run Blynk:
    ```bash
    $ sudo ./blynk --token=YourAuthToken
    ```

We have also provided a build script, you can try just running (inside of the "linux" directory):

```bash
$ ./build.sh raspberry
```
