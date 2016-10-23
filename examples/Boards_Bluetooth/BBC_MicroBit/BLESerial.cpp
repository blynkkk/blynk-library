#include "BLESerial.h"

BLESerial* BLESerial::_instance = NULL;

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

void BLESerial::begin(...) {
  BLEPeripheral::begin();
}

void BLESerial::poll() {
  BLEPeripheral::poll();
}

void BLESerial::end() {
  this->_rxCharacteristic.setEventHandler(BLEWritten, NULL);
  this->_rxHead = this->_rxTail = 0;
  BLEPeripheral::disconnect();
}

int BLESerial::available(void) {
  BLEPeripheral::poll();
  int retval = (this->_rxHead - this->_rxTail + sizeof(this->_rxBuffer)) % sizeof(this->_rxBuffer);
  return retval;
}

int BLESerial::peek(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  uint8_t byte = this->_rxBuffer[this->_rxTail];
  return byte;
}

int BLESerial::read(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  this->_rxTail = (this->_rxTail + 1) % sizeof(this->_rxBuffer);
  uint8_t byte = this->_rxBuffer[this->_rxTail];
  return byte;
}

void BLESerial::flush(void) {
  this->_rxHead = this->_rxTail = 0;
}

size_t BLESerial::write(const uint8_t* buff, size_t len) {
  this->_txCharacteristic.setValue(buff, len);
  BLEPeripheral::poll();
  return len;
}

size_t BLESerial::write(uint8_t byte) {
  Serial.print(F("BLESerial::write should not be called!"));
  for(;;);
  return -1;
}

BLESerial::operator bool() {
  bool retval = BLEPeripheral::connected();
  return retval;
}

void BLESerial::_received(const uint8_t* data, size_t size) {
  for (int i = 0; i < size; i++) {
    this->_rxHead = (this->_rxHead + 1) % sizeof(this->_rxBuffer);
    this->_rxBuffer[this->_rxHead] = data[i];
  }
}

void BLESerial::_received(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic) {
  BLESerial::_instance->_received(rxCharacteristic.value(), rxCharacteristic.valueLength());
}
