
#include <Blynk/BlynkConsole.h>

BlynkConsole    edgentConsole;

void console_init()
{
#ifdef BLYNK_PRINT
  edgentConsole.begin(BLYNK_PRINT);
#endif

  edgentConsole.print("\n>");

  edgentConsole.addCommand("reboot", []() {
    edgentConsole.print(R"json({"status":"OK","msg":"resetting device"})json" "\n");
    delay(100);
    restartMCU();
  });

  edgentConsole.addCommand("config", []() {
    edgentConsole.print(R"json({"status":"OK","msg":"entering configuration mode"})json" "\n");
    BlynkState::set(MODE_WAIT_CONFIG);
  });

  edgentConsole.addCommand("erase_config", [=]() {
    edgentConsole.print(R"json({"status":"OK","msg":"config erased"})json" "\n");
    BlynkState::set(MODE_RESET_CONFIG);
  });

  edgentConsole.addCommand("devinfo", []() {
    edgentConsole.printf(
        R"json({"name":"%s","board":"%s","tmpl_id":"%s","fw_type":"%s","fw_ver":"%s"})json" "\n",
        getWiFiName().c_str(),
        BLYNK_DEVICE_NAME,
        BLYNK_TEMPLATE_ID,
        BLYNK_FIRMWARE_TYPE,
        BLYNK_FIRMWARE_VERSION
    );
  });

  edgentConsole.addCommand("netinfo", []() {
    edgentConsole.printf(
        R"json({"ssid":"%s","bssid":"%s","mac":"%s","rssi":%d})json" "\n",
        getWiFiNetworkSSID().c_str(),
        getWiFiNetworkBSSID().c_str(),
        getWiFiMacAddress().c_str(),
        WiFi.RSSI()
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

#ifdef BLYNK_FS

  edgentConsole.addCommand("ls", [](int argc, const char** argv) {
    if (argc < 1) return;

    Dir dir = BLYNK_FS.openDir(argv[0]);
    while (dir.next()) {
      File f = dir.openFile(BLYNK_FILE_READ);

      MD5Builder md5;
      md5.begin();
      md5.addStream(f, f.size());
      md5.calculate();
      String md5str = md5.toString();

      edgentConsole.printf("%8d %-16s %s\n",
                            f.size(), dir.fileName().c_str(),
                            md5str.substring(0,8).c_str());
    }
  });

  edgentConsole.addCommand("rm", [](int argc, const char** argv) {
    if (argc < 1) return;

    for (int i=0; i<argc; i++) {
      const char* fn = argv[i];
      if (BLYNK_FS.remove(fn)) {
        edgentConsole.printf("Removed %s\n", fn);
      } else {
        edgentConsole.printf("Removing %s failed\n", fn);
      }
    }
  });

  edgentConsole.addCommand("mv", [](int argc, const char** argv) {
    if (argc != 2) return;

    if (!BLYNK_FS.rename(argv[0], argv[1])) {
      edgentConsole.print("Rename failed\n");
    }
  });

  edgentConsole.addCommand("cat", [](int argc, const char** argv) {
    if (argc != 1) return;

    if (!BLYNK_FS.exists(argv[0])) {
      edgentConsole.print("File not found\n");
      return;
    }

    if (File f = BLYNK_FS.open(argv[0], BLYNK_FILE_READ)) {
      while (f.available()) {
        edgentConsole.print((char)f.read());
      }
      edgentConsole.print("\n");
    } else {
      edgentConsole.print("Cannot open file\n");
    }
  });

  edgentConsole.addCommand("echo", [](int argc, const char** argv) {
    if (argc != 2) return;

    if (File f = BLYNK_FS.open(argv[1], BLYNK_FILE_WRITE)) {
      if (!f.print(argv[0])) {
        edgentConsole.print("Cannot write file\n");
      }
    } else {
      edgentConsole.print("Cannot open file\n");
    }
  });

#endif

}

BLYNK_WRITE(InternalPinDBG) {
  String cmd = String(param.asStr()) + "\n";
  edgentConsole.runCommand((char*)cmd.c_str());
}

