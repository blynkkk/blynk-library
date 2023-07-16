
#include <Blynk/BlynkConsole.h>

BlynkConsole    edgentConsole;

void console_init()
{
#ifdef BLYNK_PRINT
  edgentConsole.begin(BLYNK_PRINT);
#endif

  edgentConsole.print("\n>");

  edgentConsole.addCommand("reboot", []() {
    edgentConsole.print(R"json({"status":"OK","msg":"rebooting wifi module"})json" "\n");
    delay(100);
    restartMCU();
  });

  edgentConsole.addCommand("config", [](int argc, const char** argv) {
    if (argc < 1 || 0 == strcmp(argv[0], "start")) {
      BlynkState::set(MODE_WAIT_CONFIG);
    } else if (0 == strcmp(argv[0], "erase")) {
      BlynkState::set(MODE_RESET_CONFIG);
    }
  });

  edgentConsole.addCommand("devinfo", []() {
    edgentConsole.printf(
        R"json({"name":"%s","board":"%s","tmpl_id":"%s","fw_type":"%s","fw_ver":"%s"})json" "\n",
        getWiFiName().c_str(),
        BLYNK_TEMPLATE_NAME,
        BLYNK_TEMPLATE_ID,
        BLYNK_FIRMWARE_TYPE,
        BLYNK_FIRMWARE_VERSION
    );
  });

  edgentConsole.addCommand("connect", [](int argc, const char** argv) {
    if (argc < 2) {
      edgentConsole.print(R"json({"status":"error","msg":"invalid arguments. expected: <auth> <ssid> <pass>"})json" "\n");
      return;
    }
    String auth = argv[0];
    String ssid = argv[1];
    String pass = (argc >= 3) ? argv[2] : "";

    if (auth.length() != 32) {
      edgentConsole.print(R"json({"status":"error","msg":"invalid token size"})json" "\n");
      return;
    }

    edgentConsole.print(R"json({"status":"OK","msg":"trying to connect..."})json" "\n");

    configStore = configDefault;
    CopyString(ssid, configStore.wifiSSID);
    CopyString(pass, configStore.wifiPass);
    CopyString(auth, configStore.cloudToken);

    BlynkState::set(MODE_SWITCH_TO_STA);
  });

}

BLYNK_WRITE(InternalPinDBG) {
  String cmd = String(param.asStr()) + "\n";
  edgentConsole.runCommand((char*)cmd.c_str());
}

