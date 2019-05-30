#ifndef BlynkBLEPeripheralSerial_h
#define BlynkBLEPeripheralSerial_h

#include <BLEPeripheral.h>

void BLESerial_onConnect(BLECentral& central);
void BLESerial_onDisconnect(BLECentral& central);

class BLESerial : public BLEPeripheral, public Stream
{
  public:
    BLESerial(unsigned char req = BLE_DEFAULT_REQ, unsigned char rdy = BLE_DEFAULT_RDY, unsigned char rst = BLE_DEFAULT_RST);

    void begin(...);
    void poll();
    void end();

    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t byte);
    using Print::write;
    virtual operator bool();

  private:
    unsigned long _flushed;
    static BLESerial* _instance;

    size_t _rxHead;
    size_t _rxTail;
    uint8_t _rxBuffer[BLYNK_MAX_READBYTES*2];
    size_t _txCount;
    uint8_t _txBuffer[BLE_ATTRIBUTE_MAX_VALUE_LENGTH];

    BLEService _uartService = BLEService("713D0000-503E-4C75-BA94-3148F18D941E");
    BLECharacteristic _rxCharacteristic = BLECharacteristic("713D0003-503E-4C75-BA94-3148F18D941E", BLEWrite, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
    BLECharacteristic _txCharacteristic = BLECharacteristic("713D0002-503E-4C75-BA94-3148F18D941E", BLENotify, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);

    void _received(const uint8_t* data, size_t size);
    static void _received(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic);
};

BLESerial* BLESerial::_instance = NULL;

inline
BLESerial::BLESerial(unsigned char req, unsigned char rdy, unsigned char rst) :
  BLEPeripheral(req, rdy, rst)
{
  this->_txCount = 0;
  this->_rxHead = this->_rxTail = 0;
  this->_flushed = 0;
  BLESerial::_instance = this;

  addAttribute(this->_uartService);
  setAdvertisedServiceUuid(this->_uartService.uuid());
  addAttribute(this->_rxCharacteristic);
  this->_rxCharacteristic.setEventHandler(BLEWritten, BLESerial::_received);
  addAttribute(this->_txCharacteristic);
    
  this->setEventHandler(BLEConnected, BLESerial_onConnect);
  this->setEventHandler(BLEDisconnected, BLESerial_onDisconnect);
}

inline
void BLESerial::begin(...) {
  BLEPeripheral::begin();
}

inline
void BLESerial::poll() {
  if (millis() < this->_flushed + 100) {
    BLEPeripheral::poll();
  } else {
    flush();
  }
}

inline
void BLESerial::end() {
  this->_rxCharacteristic.setEventHandler(BLEWritten, NULL);
  this->_rxHead = this->_rxTail = 0;
  flush();
  BLEPeripheral::disconnect();
}

inline
int BLESerial::available(void) {
  BLEPeripheral::poll();
  int retval = (this->_rxHead - this->_rxTail + sizeof(this->_rxBuffer)) % sizeof(this->_rxBuffer);

  return retval;
}

inline
int BLESerial::peek(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  uint8_t byte = this->_rxBuffer[ (this->_rxTail + 1) % sizeof(this->_rxBuffer)];

  return byte;
}

inline
int BLESerial::read(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  this->_rxTail = (this->_rxTail + 1) % sizeof(this->_rxBuffer);
  uint8_t byte = this->_rxBuffer[this->_rxTail];

  return byte;
}

inline
void BLESerial::flush(void) {
  if (this->_txCount == 0) return;
  this->_txCharacteristic.setValue(this->_txBuffer, this->_txCount);
  this->_flushed = millis();
  this->_txCount = 0;
  BLEPeripheral::poll();
}

inline
size_t BLESerial::write(uint8_t byte) {
  BLEPeripheral::poll();
  if (this->_txCharacteristic.subscribed() == false) return 0;
  this->_txBuffer[this->_txCount++] = byte;
  if (this->_txCount == sizeof(this->_txBuffer)) flush();

  return 1;
}

inline
BLESerial::operator bool() {
  bool retval = BLEPeripheral::connected();
  
  return retval;
}

inline
void BLESerial::_received(const uint8_t* data, size_t size) {
  // BLYNK_DBG_DUMP(">> ", data, size);
  for (size_t i = 0; i < size; i++) {
    this->_rxHead = (this->_rxHead + 1) % sizeof(this->_rxBuffer);
    this->_rxBuffer[this->_rxHead] = data[i];
  }
}

inline
void BLESerial::_received(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic) {
  BLESerial::_instance->_received(rxCharacteristic.value(), rxCharacteristic.valueLength());
}

void BLESerial_onConnect(BLECentral& central) {
 BLYNK_LOG1("Device connected");
 Blynk.startSession();
}

void BLESerial_onDisconnect(BLECentral& central) {
 BLYNK_LOG1("Device disconnected");
 Blynk.disconnect();
}

#endif
