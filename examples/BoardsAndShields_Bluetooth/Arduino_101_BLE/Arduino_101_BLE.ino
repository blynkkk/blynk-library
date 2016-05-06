#define BLYNK_DEBUG
#define BLYNK_PRINT Serial

//#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleGenericBLE.h>
#include <CurieBLE.h>

char auth[] = "YourAuthToken";

void setup() {
  Serial.begin(9600);
  delay(1000);

  Blynk.begin(auth);

  blePeripheral.setLocalName("Blynk");
  blePeripheral.setAppearance(384);
  blePeripheral.setAdvertisedServiceUuid(bleService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(bleService);
  blePeripheral.addAttribute(rxChar);
  blePeripheral.addAttribute(txChar);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected,    blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  rxChar.setEventHandler(BLEWritten,    rxCharWritten);
  txChar.setEventHandler(BLESubscribed, txCharSubscribed);

  unsigned char empty[0] = {};
  rxChar.setValue(empty, 0);
  txChar.setValue(empty, 0);

  // advertise the service
  blePeripheral.begin();
  Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loop() {
  Blynk.run();
  blePeripheral.poll();
}

void blePeripheralConnectHandler(BLECentral& central) {
  BLYNK_LOG1("BLE Connected central: ");
  Serial.println(central.address());
}

void txCharSubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  BLYNK_LOG1("Subscribed");
  Blynk.bleConnect();
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  BLYNK_LOG1("BLE Disconnected");
  Blynk.bleDisconnect();
}

void rxCharWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Blynk.bleProcess(rxChar.value(), rxChar.valueLength());
}

// This is called for all virtual pins, that don't have BLYNK_WRITE handler
BLYNK_WRITE_DEFAULT() {
  BLYNK_LOG3("V", request.pin, " input: ");
  // Print all parameter values
  for (auto i = param.begin(); i < param.end(); ++i) {
    BLYNK_LOG2("* ", i.asString());
  }
  Blynk.virtualWrite(request.pin, param);
}

// This is called for all virtual pins, that don't have BLYNK_READ handler
BLYNK_READ_DEFAULT() {
  // Generate random response
  int val = random(0, 100);
  BLYNK_LOG4("V", request.pin, " output: ", val);
  Blynk.virtualWrite(request.pin, val);
}

