#ifndef BlynkBLEPeripheralSerial_h
#define BlynkBLEPeripheralSerial_h

#include <BLEPeripheral.h>

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
    size_t _rxCount() const;
    uint8_t _rxBuffer[BLE_ATTRIBUTE_MAX_VALUE_LENGTH];
    size_t _txCount;
    uint8_t _txBuffer[BLE_ATTRIBUTE_MAX_VALUE_LENGTH];

    BLEService _uartService = BLEService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
    BLEDescriptor _uartNameDescriptor = BLEDescriptor("2901", "UART");
    BLECharacteristic _rxCharacteristic = BLECharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWriteWithoutResponse, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
    BLEDescriptor _rxNameDescriptor = BLEDescriptor("2901", "RX - Receive Data (Write)");
    BLECharacteristic _txCharacteristic = BLECharacteristic("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLENotify, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
    BLEDescriptor _txNameDescriptor = BLEDescriptor("2901", "TX - Transfer Data (Notify)");

    void _received(const uint8_t* data, size_t size);
    static void _received(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic);
};

// #define BLE_SERIAL_DEBUG

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
  addAttribute(this->_uartNameDescriptor);
  setAdvertisedServiceUuid(this->_uartService.uuid());
  addAttribute(this->_rxCharacteristic);
  addAttribute(this->_rxNameDescriptor);
  this->_rxCharacteristic.setEventHandler(BLEWritten, BLESerial::_received);
  addAttribute(this->_txCharacteristic);
  addAttribute(this->_txNameDescriptor);
}

inline
void BLESerial::begin(...) {
  BLEPeripheral::begin();
  #ifdef BLE_SERIAL_DEBUG
    Serial.println(F("BLESerial::begin()"));
  #endif
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
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("BLESerial::available() = "));
    Serial.println(retval);
  #endif
  return retval;
}

inline
int BLESerial::peek(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  uint8_t byte = this->_rxBuffer[ (this->_rxTail + 1) % sizeof(this->_rxBuffer)];
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("BLESerial::peek() = "));
    Serial.print((char) byte);
    Serial.print(F(" 0x"));
    Serial.println(byte, HEX);
  #endif
  return byte;
}

inline
int BLESerial::read(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  this->_rxTail = (this->_rxTail + 1) % sizeof(this->_rxBuffer);
  uint8_t byte = this->_rxBuffer[this->_rxTail];
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("BLESerial::read() = "));
    Serial.print((char) byte);
    Serial.print(F(" 0x"));
    Serial.println(byte, HEX);
  #endif
  return byte;
}

inline
void BLESerial::flush(void) {
  if (this->_txCount == 0) return;
  this->_txCharacteristic.setValue(this->_txBuffer, this->_txCount);
  this->_flushed = millis();
  this->_txCount = 0;
  BLEPeripheral::poll();
  #ifdef BLE_SERIAL_DEBUG
    Serial.println(F("BLESerial::flush()"));
  #endif
}

inline
size_t BLESerial::write(uint8_t byte) {
  BLEPeripheral::poll();
  if (this->_txCharacteristic.subscribed() == false) return 0;
  this->_txBuffer[this->_txCount++] = byte;
  if (this->_txCount == sizeof(this->_txBuffer)) flush();
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("BLESerial::write("));
    Serial.print((char) byte);
    Serial.print(F(" 0x"));
    Serial.print(byte, HEX);
    Serial.println(F(") = 1"));
  #endif
  return 1;
}

inline
BLESerial::operator bool() {
  bool retval = BLEPeripheral::connected();
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("BLESerial::operator bool() = "));
    Serial.println(retval);
  #endif
  return retval;
}

inline
void BLESerial::_received(const uint8_t* data, size_t size) {
  for (int i = 0; i < size; i++) {
    this->_rxHead = (this->_rxHead + 1) % sizeof(this->_rxBuffer);
    this->_rxBuffer[this->_rxHead] = data[i];
  }
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("BLESerial::received("));
    for (int i = 0; i < size; i++) Serial.print((char) data[i]);
    Serial.println(F(")"));
  #endif
}

inline
void BLESerial::_received(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic) {
  BLESerial::_instance->_received(rxCharacteristic.value(), rxCharacteristic.valueLength());
}

#endif
