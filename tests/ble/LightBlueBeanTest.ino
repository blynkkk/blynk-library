
void setup()
{
  Serial.begin(57600);
  Serial.setTimeout(25);
  // Turn off the LED
  Bean.setLed(0, 0, 0);
  Bean.enableWakeOnConnect(true);
}


void loop()
{
  while (Bean.getConnectionState())
  {
    // Turn the LED green
    Bean.setLed(0, 255, 0);
    while (Serial.available())
    {
      Bean.setLed(0, 0, 255);
      char buff[64];
      int len = Serial.readBytes(buff, 64);
      Serial.write((byte*)buff, len);
    }
    Bean.sleep(100);
  }
  // Turn off the LED
  Bean.setLed(0, 0, 0);
  // Sleep forever or until the Bean wakes up by being connected to
  Bean.sleep(0xFFFFFFFF);
}

