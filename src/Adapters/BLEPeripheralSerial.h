#ifndef _BLE_SERIAL_H_
#define _BLE_SERIAL_H_

#include <BLEPeripheral.h>

class BLESerial : public BLEPeripheral, public Stream
{
  public:
    BLESerial(unsigned char req, unsigned char rdy, unsigned char rst);

    void begin(...);
    void poll();
    void end();

    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t byte);
    virtual size_t write(const uint8_t *buff, size_t len);
    using Print::write;
    virtual operator bool();

  private:
    static BLESerial* _instance;

    size_t _rxHead;
    size_t _rxTail;
    size_t _rxCount() const;
    uint8_t _rxBuffer[256];

    BLEService _uartService = BLEService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
    BLEDescriptor _uartNameDescriptor = BLEDescriptor("2901", "UART");
    BLECharacteristic _rxCharacteristic = BLECharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWriteWithoutResponse | BLEWrite, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
    BLEDescriptor _rxNameDescriptor = BLEDescriptor("2901", "RX - Receive Data (Write)");
    BLECharacteristic _txCharacteristic = BLECharacteristic("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLERead | BLENotify, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
    BLEDescriptor _txNameDescriptor = BLEDescriptor("2901", "TX - Transfer Data (Notify)");

    void _received(const uint8_t* data, size_t size);
    static void _received(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic);
};

static BLESerial* BLESerial::_instance = NULL;

static inline
BLESerial::BLESerial(unsigned char req, unsigned char rdy, unsigned char rst) :
  BLEPeripheral(req, rdy, rst)
{
  this->_rxHead = this->_rxTail = 0;
  BLESerial::_instance = this;

  addAttribute(this->_uartService);
  addAttribute(this->_uartNameDescriptor);
  setAdvertisedServiceUuid(this->_uartService.uuid());
  addAttribute(this->_rxCharacteristic);
  //addAttribute(this->_rxNameDescriptor);
  addAttribute(this->_txCharacteristic);
  //addAttribute(this->_txNameDescriptor);
  this->_rxCharacteristic.setEventHandler(BLEWritten, BLESerial::_received);
}

static inline
void BLESerial::begin(...) {
  BLEPeripheral::begin();
}

static inline
void BLESerial::poll() {
  BLEPeripheral::poll();
}

static inline
void BLESerial::end() {
  this->_rxCharacteristic.setEventHandler(BLEWritten, NULL);
  this->_rxHead = this->_rxTail = 0;
  BLEPeripheral::disconnect();
}

static inline
int BLESerial::available(void) {
  BLEPeripheral::poll();
  int retval = (this->_rxHead - this->_rxTail + sizeof(this->_rxBuffer)) % sizeof(this->_rxBuffer);
  return retval;
}

static inline
int BLESerial::peek(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  uint8_t byte = this->_rxBuffer[this->_rxTail];
  return byte;
}

static inline
int BLESerial::read(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  this->_rxTail = (this->_rxTail + 1) % sizeof(this->_rxBuffer);
  uint8_t byte = this->_rxBuffer[this->_rxTail];
  return byte;
}

static inline
void BLESerial::flush(void) {
  this->_rxHead = this->_rxTail = 0;
}

static inline
size_t BLESerial::write(const uint8_t* buff, size_t len) {
  this->_txCharacteristic.setValue(buff, len);
  BLEPeripheral::poll();
  return len;
}

static inline
size_t BLESerial::write(uint8_t byte) {
  Serial.print(F("BLESerial::write should not be called!"));
  for(;;);
  return -1;
}

static inline
BLESerial::operator bool() {
  bool retval = BLEPeripheral::connected();
  return retval;
}

static inline
void BLESerial::_received(const uint8_t* data, size_t size) {
  for (int i = 0; i < size; i++) {
    this->_rxHead = (this->_rxHead + 1) % sizeof(this->_rxBuffer);
    this->_rxBuffer[this->_rxHead] = data[i];
  }
}

static inline
void BLESerial::_received(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic) {
  BLESerial::_instance->_received(rxCharacteristic.value(), rxCharacteristic.valueLength());
}

#endif
