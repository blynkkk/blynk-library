Instruction for Blynk Demo App - **myPlant**.

# Setting up Arduino IDE

1. Install latest [Arduino IDE](https://www.arduino.cc/en/Main/Software).

2. Download latest Blynk release package: [Blynk_Release_v0.3.10.zip](https://github.com/blynkkk/blynk-library/releases/download/v0.3.10/Blynk_Release_v0.3.10.zip).
   It contains 4 libraries, please copy them to Arduino Libraries folder. 
   If you don’t know how to do this, please follow manual Installation [guide](https://www.arduino.cc/en/Guide/Libraries#toc5).
   
3. Open Arduino IDE, then select in menu: **File -> Preferences**.
   Click button **Additional Boards Manager URLs** and add this link:
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ![File -> Preferences](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/file_preferences.png)
   
4. In Arduino IDE menu, open: **Tools -> Board -> Boards Manager**.
   Search for ```esp8266```, and install it:
   ![Tools -> Board -> Boards Manager](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/boards_manager.png)
   
# Uploading App Export example

1. Open **File -> Examples -> Blynk -> Export_Demo -> myPlant_ESP8266**

2. Now select **Tools -> Board -> NodeMCU 1.0 (ESP-12E Module)**.

   **Note:** This board selection works for SparkFun Blynk Board and NodeMCU as well.

3. If you’re using Node MCU, uncomment ```#define USE_NODE_MCU_BOARD``` in the ```myPlant_ESP8266.ino``` file.

4. Click **Verify** and check if everything went OK:

   ![Verify](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/verify.png)
   
5. Check what ports are available in your **Tools -> Board** menu.
   Connect your board, and select the new port that appeared in **Tools -> Board** menu.
   
6. Click **Upload**. (You should see some progress bar in the console, and then **Done uploading** message on the 
   status panel. After this process is finished, RGB status led should start blinking blue: the board is waiting for configuration).
   
7. Unplug the board. (You will plug the power again when configuring the board. Imagine that you have just unboxed your product).

# Running Blynk myPlant example

1. Download Blynk myPlant app (todo : add link) from Google Play, and run it.

2. Follow the steps in WiFi provisioning wizard to connect the hardware to your WiFi network:
   
   a. Make sure your hardware is powered up (just connect the USB cable). RGB led should be blue and blink fast.
   b. Click Begin to start device configuration :
   ![Step 1](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/provisioning_start.png)
   c. Choose your home WiFi name and password : 
   ![Step 2](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/provisioning_choose_network.png)
   d. Choose device, that you want to configure :
   ![Step 3](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/provisioning_connect.png)
   e. Click **DONE** : 
   ![Step 4](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/provisioning_done.png)
   f. Enjoy :
   ![Step 5](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/demo_app.png)

3. After the board is connected, the app is ready to be used.
   Please note that this is only emulating a plant watering device, as this simple demo would require you to 
   connect all of the modules to create a functional solution.

# Resetting board configuration

**NodeMCU**: Press and hold the **FLASH** button (near USB connector) for 10 seconds.
**SparkFun Blynk Board**: Press and hold the on board button “0” for 10 seconds.
After few seconds it will start fading white color, and then turn solid white. 
Then you can release the button to reset device configuration.
After the board configuration is reset, you can configure it again from the **myPlant App**.

# Troubleshooting

## RGB indicator problems

If you are using NodeMCU, and your RGB led shows wrong colors (or doesn’t light-up at all), it may be possible 
that your LED is with “common anode”. Here is a good picture to explain it :

![RGB](http://bildr.org/blog/wp-content/uploads/2011/01/RGBLEDs2.png)

In this case, you will have to:

  1. Connect the longest LED pin to the VCC instead of GND.
  2. Change BOARD_LED_INVERSE to true for your board in Settings.h tab
  
## No ports appear, when board is plugged in

For Windows or OS X you may need to install drivers [here](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx) 
or [here](http://www.ftdichip.com/Drivers/VCP.htm).

For Ubuntu / linux, it may happen that the board is detected, but when you try flashing the board, 
an error appears: ```Error opening serial port...```
If you get this error, you need to set serial port permission. Open terminal, and run command:

```sudo usermod -a -G dialout $USER```

You will need to reboot (or just log out and log in) for this change to take effect.