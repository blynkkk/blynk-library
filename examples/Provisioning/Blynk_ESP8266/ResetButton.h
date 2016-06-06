

void button_change(void)
{
  static unsigned long buttonPressTime = 0;
  if (digitalRead(BOARD_BUTTON_PIN)) // Button rising - released
  {
    //runMode = previousMode; // Leave button press mode
    //indicatorRestart();
    DEBUG_PRINT("Button released");
    

      unsigned long buttonHoldTime = millis() - buttonPressTime;
      if (buttonHoldTime >= BUTTON_HOLD_TIME_MIN)
      {
        // If the button has been held for minimum time (3s)
        // execute the button release code:
        //buttonRelease();
      }
  }
  else // Button falling - pressed
  {
    DEBUG_PRINT("Button pressed");
    /*buttonPressTime = millis(); // Log the press time
    if ((runMode == MODE_WAIT_CONFIG) || (runMode == MODE_CONFIG) ||
        (runMode == MODE_CONNECTING_WIFI) || (runMode == MODE_CONNECTING_BLYNK))
    { // If in config, wait-for-config mode, or trying to connect
      previousMode = runMode; // Store current mode
      runMode = MODE_BUTTON_HOLD; // set to button-hold mode
    }*/
  }
}

// TODO: debounce
void button_init(){
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(BOARD_BUTTON_PIN, button_change, CHANGE);
}


