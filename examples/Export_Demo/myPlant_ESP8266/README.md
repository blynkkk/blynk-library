Instruction for Blynk Demo App - **myPlant**.

# Setting up Arduino IDE

1. Install latest [Arduino IDE](https://www.arduino.cc/en/Main/Software).

2. Download latest Blynk release package: [Blynk_Release_v0.3.10.zip](https://github.com/blynkkk/blynk-library/releases/download/v0.3.10/Blynk_Release_v0.3.10.zip).
   It contains 5 libraries, please copy them to Arduino Libraries folder. 
   If you don’t know how to do this, please follow manual installation [guide](https://www.arduino.cc/en/Guide/Libraries#toc5).
   
3. Open the Arduino IDE, then select in the menu: **File -> Preferences**.  
   Click on **Additional Boards Manager URLs** button and add this link:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
   ![File -> Preferences](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/file_preferences.png)
   
4. In the Arduino IDE menu open: **Tools -> Board -> Boards Manager**.  
   Search for ```esp8266```, and install it:
   ![Tools -> Board -> Boards Manager](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/boards_manager.png)
   
# Uploading Blynk Demo App example

1. Open **File -> Examples -> Blynk -> Export_Demo -> myPlant_ESP8266**

2. In **Tools -> Board** menu, select **NodeMCU 1.0 (ESP-12E Module)**.  
   **Note:** This board selection works for both SparkFun BlynkBoard and NodeMCU.

3. If you use NodeMCU, uncomment ```#define USE_NODE_MCU_BOARD``` in the ```myPlant_ESP8266.ino``` file.

4. Click on **Verify** button and make sure that example compiled correctly:

   ![Verify](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/verify.png)
   
5. Select the port of your board in **Tools -> Port** menu.  
   In **Tools -> Upload Speed**, select **115200** (higher values may also work for you).  
   If you don't know how to select the port, check this [troubleshooting section](#how-to-pick-the-correct-port).

6. Click the **Upload** button. (You should see a progress bar in the console, followed by **Done uploading** message in the 
   status panel. After this process is finished, LED should start blinking blue: which means the board is waiting for configuration).
   
   
The firmware was successfully loaded and is ready. 

You can switch the board off and then on again. Imagine you just unboxed the new product. LED should be blinking blue.


# Running Blynk myPlant Demo

1. Download [Blynk myPlant app](https://play.google.com/store/apps/details?id=cc.blynk.appexport.demo&hl=en) from Google Play, and open it.

2. Follow the steps in WiFi provisioning wizard to connect the hardware to your WiFi network:
   
   * Make sure your hardware is powered up (with the battery or USB cable). Status LED should be blinking blue. It means that at this moment the board acts as an Access Point and broadcasts it's own WiFi network
   * In the app click **Begin** to start device provisioning:
   
   ![Step 1](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/provisioning_start.png)
   
   * Choose the WiFi SSID you would like your device be connected to and input the password for it. It can be your home WiFi network.
    
   **Note**: you may be asked for a permission to access GPS. It's a regular Android OS permission to get the hardware identifiers during scanning for WiFi networks.
   
   ![Step 2](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/provisioning_choose_network.png)
   
   * Choose **Blynk myPlant** device in the list of networks. This is the board which will be configured. 
   
   ![Step 3](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/provisioning_connect.png)
   
   Now your smartphone is connected to the Access Point created by the device.
   You should see that status LED is blinking blue, but slower. Now the provisioning process starts.
   
   During this process:
   - smartphone passes WiFi credentials to the device.
   - smartphone requests new Auth Token from the server and passes it to the device
   - this data is stored in the device's memory (EEPROM)
   
   When the data was successfully transfered to the device:
   - Device automatically rebooots
   - Uses the stored WiFi credentials to connects to the WiFi network you specified 
   - Uses unique Auth Token to connect to Blynk Cloud  
   
   After device successfully connected to Blynk Cloud  status LED will change to "breathing green". 
   
   * Click **DONE**: 
   
   ![Step 4](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/provisioning_done.png)

3. The app is ready to be used.
   
   ![App Ready](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/demo_app.png)
   
   **Note:** This is a simulation of a plant watering device. The app is communicating with the hardware, but the data is not real.
   
# RGB LED status indication

![0](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/states/0.png)  Fast Blinking Blue: WAIT_CONFIG mode - Device acts as an Access Point (broadcasts it's own WiFi network). It's waiting for the app to pass the required information

![1](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/states/1.png)  CONFIGURING mode - device is getting the data from the smartphone

![2](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/states/2.png)  Blinking Blynk Green: CONNECTING_NET mode - device is connecting to the WiFi network

![3](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/states/3.png)  Fast Blinking Blynk Green: CONNECTING_CLOUD - device is trying to connect to Blynk Cloud

![4](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/states/4.png)  Breathing Blynk Green: RUNNING - device successfully connected to Blynk Cloud

![6](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/states/6.png)  Blinking Red: ERROR mode

![7](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/states/7.png)  Breathing White - confirming resetting the board configuration

![8](https://github.com/blynkkk/blynk-library/blob/master/extras/docs/images/states/8.png)  Blinking White - board configuration was reset

# Resetting the board

There might be situations when you would need reset the board in case something went wrong, or you would like to connect it to a different WiFi network. 

**NodeMCU**: Press and hold the **FLASH** button (near USB connector) for 10 seconds.

**SparkFun Blynk Board**: Press and hold the on board button “0” for 10 seconds.

After a few seconds status LED will change to breathing white color, and then turn to blinking white. 
Release the button. Device will be reset and you can start the provisioning process again using the **myPlant App**.

# Troubleshooting

## LED issues

If you are using NodeMCU, and your RGB LED shows incorrect colors (or doesn’t light-up at all), there is a chance that your LED is a “common anode” LED as shown on the image below:

![RGB](http://bildr.org/blog/wp-content/uploads/2011/01/RGBLEDs2.png)

In this case, you will have to:

  1. Connect the longest LED pin to the ```VCC``` instead of ```GND```.
  2. Change ```BOARD_LED_INVERSE``` to ```true``` for your board in ```Settings.h``` tab
  
## No ports appear, when board is plugged in

For Windows or OS X you may need to install drivers [here](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx) 
or [here](http://www.ftdichip.com/Drivers/VCP.htm).

For Ubuntu / linux, it may happen that the board is detected, but when you try flashing the board, 
an error appears: ```Error opening serial port...```
If you get this error, you need to set serial port permission. Open terminal, and run command:

```bash
sudo usermod -a -G dialout $USER
```

**Note:** You will need to reboot (or just log out and log in) for this change to take effect.

## How to pick the correct port?
1. Ensure that your board is disconnected.
2. Remember which ports are available in your **Tools -> Board** menu.
3. Connect your board and wait 30 seconds.
4. Select the new port that appeared in **Tools -> Board** menu (the one that was missing before).

## The board doesn't connect to home network, status LED flashes red and board restarts.
Ensure you have entered valid WiFi configuration (picked correct WiFi name and password).
If you make a mistake, ho have to [reset the board](#resetting-the-board)
