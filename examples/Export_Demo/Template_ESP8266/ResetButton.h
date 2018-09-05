/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/

volatile bool     g_buttonState = false;
volatile bool     g_buttonPressed = false;
volatile uint32_t g_buttonPressTime = -1;

void button_action(void)
{
  BlynkState::set(MODE_RESET_CONFIG);
}

void button_change(void)
{
#if BOARD_BUTTON_ACTIVE_LOW
  g_buttonState = !digitalRead(BOARD_BUTTON_PIN);
#else
  g_buttonState = digitalRead(BOARD_BUTTON_PIN);
#endif

if (!g_buttonPressed && g_buttonState) {
    g_buttonPressed = true;
    DEBUG_PRINT("Button pressed...");
    g_buttonPressTime = millis();
  } else if (g_buttonPressed && !g_buttonState) {
    g_buttonPressed = false;
    uint32_t buttonHoldTime = millis() - g_buttonPressTime;
    DEBUG_PRINT(String("Button released. Hold time: ") + String(buttonHoldTime));
    if (buttonHoldTime >= BUTTON_HOLD_TIME_ACTION) {
      button_action();
    }
    g_buttonPressTime = -1;
  }
}

void button_init()
{
#if BOARD_BUTTON_ACTIVE_LOW
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLUP);
#else
  pinMode(BOARD_BUTTON_PIN, INPUT);
#endif
  attachInterrupt(BOARD_BUTTON_PIN, button_change, CHANGE);
}

