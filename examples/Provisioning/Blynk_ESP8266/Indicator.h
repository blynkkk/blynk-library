/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                    http://tiny.cc/BlynkB2B
 *
 **************************************************************/

#if BOARD_LED_IS_WS2812
  #include <Adafruit_NeoPixel.h>
  Adafruit_NeoPixel rgb = Adafruit_NeoPixel(1, BOARD_LED_PIN, NEO_GRB + NEO_KHZ800);
#endif

void indicator_run();

#define DIMM(x)    (((x)*(BOARD_RGB_BRIGHTNESS))/255)
#define RGB(r,g,b) (DIMM(r) << 16 | DIMM(g) << 8 | DIMM(b) << 0)

class Indicator {
public:

  enum Colors {
    COLOR_BLACK   = RGB(0x00, 0x00, 0x00),
    COLOR_WHITE   = RGB(0xFF, 0xFF, 0xE7),
    COLOR_BLUE    = RGB(0x0D, 0x36, 0xFF),
    COLOR_BLYNK   = RGB(0x2E, 0xFF, 0xB9),
    COLOR_RED     = RGB(0xFF, 0x10, 0x08),
    COLOR_MAGENTA = RGB(0xA7, 0x00, 0xFF),
  };
  
  Indicator() {
    m_Counter = 0;
    initLED();
  }

  uint32_t run() {
    if (g_buttonPressed) {
      if (millis() - g_buttonPressTime > BUTTON_HOLD_TIME_MAX) { return beatLED(COLOR_WHITE,   (int[]){ 100, 100 }); }
      if (millis() - g_buttonPressTime > BUTTON_HOLD_TIME_MIN) { return waveLED(COLOR_WHITE,   1000); }
    }
    switch (BlynkState::get()) {
    case MODE_WAIT_CONFIG:       return beatLED(COLOR_BLUE,    (int[]){ 50, 500 });
    case MODE_CONFIGURING:       return beatLED(COLOR_BLUE,    (int[]){ 200, 200 });
    case MODE_CONNECTING_NET:    return beatLED(COLOR_BLYNK,   (int[]){ 50, 500 });
    case MODE_CONNECTING_CLOUD:  return beatLED(COLOR_BLYNK,   (int[]){ 100, 100 });
    case MODE_RUNNING:           return waveLED(COLOR_BLYNK,   5000);
    case MODE_OTA_UPGRADE:       return beatLED(COLOR_MAGENTA, (int[]){ 50, 50 });
    default:                     return beatLED(COLOR_RED,     (int[]){ 80, 100, 80, 1000 } );
    }
  }

  void updateState() {
    m_Counter = 0;
    indicator_run();
  }

protected:

#if BOARD_LED_IS_WS2812

  void initLED() {
    rgb.begin();
    setRGB(COLOR_BLACK);
  }

  void setRGB(uint32_t color) {
    rgb.setPixelColor(0, color);
    rgb.show();
  }

  template<typename T>
  int beatLED(uint32_t onColor, const T& beat) {
    const uint8_t cnt = sizeof(beat)/sizeof(beat[0]);
    setRGB((m_Counter % 2 == 0) ? onColor : COLOR_BLACK);
    uint32_t next = beat[m_Counter];
    m_Counter = (m_Counter+1) % cnt;
    return next;
  }

  uint32_t waveLED(uint32_t colorMax, unsigned breathePeriod) {
    uint8_t redMax = (colorMax & 0xFF0000) >> 16;
    uint8_t greenMax = (colorMax & 0x00FF00) >> 8;
    uint8_t blueMax = (colorMax & 0x0000FF);
  
    // Brightness will rise from 0 to 128, then fall back to 0
    uint8_t brightness = (m_Counter < 128) ? m_Counter : 255 - m_Counter;
  
    // Multiply our three colors by the brightness:
    redMax *= ((float)brightness / 128.0);
    greenMax *= ((float)brightness / 128.0);
    blueMax *= ((float)brightness / 128.0);
    // And turn the LED to that color:
    setRGB((redMax << 16) | (greenMax << 8) | blueMax);
  
    // This function relies on the 8-bit, unsigned m_Counter rolling over. 
    m_Counter = (m_Counter+1) % 256;
    return breathePeriod / 256;
  }

#else

  void initLED() {
    pinMode(BOARD_LED_PIN, OUTPUT);
  }

  void setLED(uint8_t color) {
    analogWrite(BOARD_LED_PIN, color);
  }

  template<typename T>
  int beatLED(uint32_t, const T& beat) {
    const uint8_t cnt = sizeof(beat)/sizeof(beat[0]);
    setLED((m_Counter % 2 == 0) ? 255 : 0);
    uint32_t next = beat[m_Counter];
    m_Counter = (m_Counter+1) % cnt;
    return next;
  }

  uint32_t waveLED(uint32_t, unsigned breathePeriod) {  
    uint8_t brightness = (m_Counter < 128) ? m_Counter : 255 - m_Counter;

    setLED(255 * ((float)brightness / 128.0));

    // This function relies on the 8-bit, unsigned m_Counter rolling over. 
    m_Counter = (m_Counter+1) % 256;
    return breathePeriod / 256;
  }

#endif 

private:
  uint8_t m_Counter;
};

Indicator indicator;

#if defined(USE_TICKER)

  #include <Ticker.h>
  Ticker blinker;

  void indicator_run() {
    uint32_t returnTime = indicator.run();
    if (returnTime) {
      blinker.attach_ms(returnTime, indicator_run);
    }
  }
  
  void indicator_init() {
    blinker.attach_ms(100, indicator_run);
  }

#elif defined(USE_TIMER_ONE)

  #include <TimerOne.h>

  void indicator_run() {
    uint32_t returnTime = indicator.run();
    if (returnTime) {
      Timer1.initialize(returnTime*1000);
    }
  }

  void indicator_init() {
    Timer1.initialize(100*1000);
    Timer1.attachInterrupt(indicator_run);
  }

#elif defined(USE_TIMER_THREE)

  #include <TimerThree.h>

  void indicator_run() {
    uint32_t returnTime = indicator.run();
    if (returnTime) {
      Timer3.initialize(returnTime*1000);
    }
  }

  void indicator_init() {
    Timer3.initialize(100*1000);
    Timer3.attachInterrupt(indicator_run);
  }

#else

  #warning LED indicator needs a functional timer!

  void indicator_run() {}
  void indicator_init() {}

#endif

