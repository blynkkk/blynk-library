# Linux Quick Start Guide (Ubuntu, Raspberry Pi, ...)

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

To build a dynamic library that can be installed system-wide, try:
```bash
$ make target=raspberry libblynk.so
$ sudo make install
```

Then see ./library-test/ for a small sample code on how to use the library in your own project.