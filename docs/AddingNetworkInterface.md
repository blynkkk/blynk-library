### Adding network interface

4 entities should be created to add a new network interface to Blynk:

1. Select connection interface that will be used for Blynk operation.  
  This should be something like http://www.arduino.cc/en/Tutorial/WebClient  
  Based on the API of the connection, create the **Transport**.  
  Some examples may be found in the Adapters folder:
  * BlynkTransportSerial
  * BlynkTransportCC3000
  * BlynkArduinoClient - *can be reused, if possible*
  
2. Create **Blynk representative class**, which contains connection-specific helper functions (like begin).
  Examples:
  * BlynkEthernet
  * BlynkSerial
  * BlynkCC3000
  * BlynkWildFire
  * BlynkYun
  
3. Create **BlynkSimple*** header for your connection.  
  This constructs main **Blynk instance**, so the user (mostly) doesn't need to get into such details.  
  Examples:
  * BlynkSimpleEthernet.h
  * BlynkSimpleCC3000.h
  * BlynkSimpleWifi.h
  * BlynkSimpleUIPEthernet.h
  
4. Create a **simple example** for your platform ;)
