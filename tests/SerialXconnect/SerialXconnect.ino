/*
 * This utility allows you to cross-connect two serial channels
 * using any Arduino-compatible hardware.
 *
 * This is usefull for:
 *   Checking wiring
 *   Converting baud rate
 *   Sending AT commands
 *   etc.
 *
 * Popular AT command sets:
 *   ESP8266: http://www.espressif.com/sites/default/files/4a-esp8266_at_instruction_set_en_v1.5.4_0.pdf
 *   SIM800L: https://www.itead.cc/wiki/images/6/6f/SIM800_Series_AT_Command_Manual_V1.05.pdf
 *   XBee:    https://cdn.sparkfun.com/learn/materials/29/22AT%20Commands.pdf
 */

#define ser1 Serial

#if defined(HAVE_HWSERIAL1)
  #define ser2 Serial1
#else
  #warning Using Software Serial!
  #include <SoftwareSerial.h>
  SoftwareSerial SerialSw(2, 3);
  #define ser2 SerialSw
#endif

void setup() {
  ser1.begin(9600);
  while (!ser1);
  delay(10);
  ser2.begin(9600);
  delay(10);
}

void loop() {
  if (ser1.available()) {
    int c = ser1.read();
    if (!parseCommands(c)) {
      ser2.write(c);
    }
  }
  if (ser2.available()) {
    ser1.write(ser2.read());
  }
}

/*
 * Special commands:
 *   !speed=1000000 (after ESP8266 command: AT+UART_CUR=1000000,8,1,0,0)
 */

char cmd_prefix[] = "!speed=";
uint8_t cmd_prefix_idx = 0;

bool parseCommands(int c) {
  if (c == cmd_prefix[cmd_prefix_idx]) {
    cmd_prefix_idx++;
    if (cmd_prefix_idx >= sizeof(cmd_prefix) - 1) {
      String cmd = ser1.readStringUntil('\n');
      long baud = cmd.toInt();
      ser1.print("Speed:"); ser1.println(baud);
      ser2.begin(baud);
      cmd_prefix_idx = 0;
    }
    return true;
  } else if (cmd_prefix_idx > 0) {
    ser2.write(cmd_prefix, cmd_prefix_idx);
    cmd_prefix_idx = 0;
  }
  return false;
}

