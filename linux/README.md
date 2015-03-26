
To build on Raspberry Pi:

1. Install WiringPi:
    http://wiringpi.com/download-and-install/

2. Download and build Blynk:

        git checkout https://github.com/blynkkk/blynk-library.git
        cd blynk-library/linux
        make clean all target=raspberry

3. Run Blynk:

        sudo ./blynk --token=YourAuthToken
