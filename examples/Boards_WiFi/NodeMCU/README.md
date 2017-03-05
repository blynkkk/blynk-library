# Instruction for NodeMCU setup.

# Setting up Arduino IDE

1. Install latest [Arduino IDE](https://www.arduino.cc/en/Main/Software).

2. **Blynk library should be installed manually**.
   - Download latest Blynk library: [Blynk_Release_v0.4.4.zip](https://github.com/blynkkk/blynk-library/releases/download/v0.4.4/Blynk_Release_v0.4.4.zip). 
   - Unzip it. This archive contains several libraries.
   - Copy them to  **sketchbook folder** of Arduino IDE. To find exact **sketchbook folder** location, go to `File -> Preferences` (`Arduino -> Preferences` for Mac) top menu in Arduino IDE.
   
   The structure of your sketchbook folder should look like this, along with your saved sketches:
   
   ```
   sketchbook/libraries/Blynk
   sketchbook/libraries/BlynkESP8266_Lib
   ...
   sketchbook/tools/BlynkUpdater
   sketchbook/tools/BlynkUsbScript
   ...
   ```
   
3. Open the Arduino IDE, then select in the menu: `File -> Preferences` (`Arduino -> Preferences` for Mac).  
   Click on **Additional Boards Manager URLs** button and add this link:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
   ![File -> Preferences](/extras/docs/images/file_preferences.png)
   
4. In the Arduino IDE menu open: **Tools -> Board -> Boards Manager**.  
   Search for ```esp8266```, and install it:
   ![Tools -> Board -> Boards Manager](/extras/docs/images/boards_manager.png)
   
   In case you see ```Error downloading http://downloads.arduino.cc/packages/package_index.json``` like this : 
      
      ![Error Download](/extras/docs/images/error_download_mac.png)
      
   You need to find folder ```Arduino15``` or ```.Arduino15``` and remove all files inside with ```package``` word in name.
   
# Uploading NodeMCU Blynk example

1. Open **File -> Examples -> Blynk -> Boards_WiFi -> NodeMCU**

   ![File -> Examples -> Blynk -> Boards_WiFi -> NodeMCU](/extras/docs/images/nodeMCU_in_menu.png)
   
2. Put your auth token from Blynk app and your WiFi credentials in sketch : 

   ```
    // You should get Auth Token in the Blynk App.
    // Go to the Project Settings (nut icon).
    char auth[] = "YourAuthToken";

    // Your WiFi credentials.
    // Set password to "" for open networks.
    char ssid[] = "YourNetworkName";
    char pass[] = "YourPassword";
   ```

3. In **Tools -> Board** menu, select **NodeMCU 1.0 (ESP-12E Module)**.

   ![Tools -> Board](/extras/docs/images/nodemcu_board_type.png)

4. Click on **Verify** button and make sure that example compiled correctly:

   ![Verify](/extras/docs/images/verify.png)
   
5. Select the port of your board in **Tools -> Port** menu.  
   In **Tools -> Upload Speed**, select **115200** (higher values may also work for you).  
   If you don't know how to select the port check this [troubleshooting section](#how-to-pick-the-correct-port) or 
   if you don't see any ports, check this [troubleshooting section](#no-ports-appear-when-board-is-plugged-in).

6. Click the **Upload** button. (You should see a progress bar in the console, followed by **Done uploading** message in the 
   status panel. After this process is finished, LED should start blinking blue: which means the board is waiting for configuration).
   
The firmware was successfully loaded and is ready. Now go to app and try to light on board LED (pin D4).

# Troubleshooting

## Arduino IDE upload issue

During sketch upload you may see next errors : 

```
error: espcomm_open failed
error: espcomm_upload_mem failed
```

This means you have no drivers installed for. Follow [this](https://github.com/blynkkk/blynk-library/blob/master/examples/Export_Demo/myPlant_ESP8266/README.md#no-ports-appear-when-board-is-plugged-in).  

## LED issues

If you are using NodeMCU, and your RGB LED shows incorrect colors (or doesn’t light-up at all), there is a chance that your LED is a “common anode” LED as shown on the image below:

![RGB](http://bildr.org/blog/wp-content/uploads/2011/01/RGBLEDs2.png)

In this case, you will have to:

  1. Connect the longest LED pin to the ```VCC``` instead of ```GND```.
  2. Change ```BOARD_LED_INVERSE``` to ```true``` for your board in ```Settings.h``` tab
  
## No ports appear, when board is plugged in

For Windows or OS X you may need to install drivers [here](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx) 
or [here](http://www.ftdichip.com/Drivers/VCP.htm) or for Max [here](https://blog.sengotta.net/signed-mac-os-driver-for-winchiphead-ch340-serial-bridge/).

For Ubuntu / linux, it may happen that the board is detected, but when you try flashing the board, 
an error appears: ```Error opening serial port...```
If you get this error, you need to set serial port permission. Open terminal, and run command:

```bash
sudo usermod -a -G dialout $USER
```

When drivers are installed and you still don't see port try to unplug and plug USB for your NodeMCU board. It should help.

**Note:** You will need to reboot (or just log out and log in) for this change to take effect.

## How to pick the correct port?
1. Ensure that your board is disconnected.
2. Remember which ports are available in your **Tools -> Board** menu.
3. Connect your board and wait 30 seconds.
4. Select the new port that appeared in **Tools -> Board** menu (the one that was missing before).

## The board doesn't connect to home network, status LED flashes red and board restarts.
Ensure you have entered valid WiFi configuration (picked correct WiFi name and password).
If you make a mistake, ho have to [reset the board](#resetting-the-board)
