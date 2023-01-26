
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

  edgentConsole.addCommand("wifi", [](int argc, const char* argv[]) {
    if (argc < 1 || 0 == strcmp(argv[0], "show")) {
      edgentConsole.printf(
          "mac:%s ip:%s (%s [%s] %ddBm)\n",
          getWiFiMacAddress().c_str(),
          WiFi.localIP().toString().c_str(),
          getWiFiNetworkSSID().c_str(),
          getWiFiNetworkBSSID().c_str(),
          WiFi.RSSI()
      );
    } else if (0 == strcmp(argv[0], "scan")) {
      int found = WiFi.scanNetworks();
      for (int i = 0; i < found; i++) {
        bool current = (WiFi.SSID(i) == WiFi.SSID());
        edgentConsole.printf(
            "%s %s [%s] %s ch:%d rssi:%d\n",
            (current ? "*" : " "), WiFi.SSID(i).c_str(),
            macToString(WiFi.BSSID(i)).c_str(),
            wifiSecToStr(WiFi.encryptionType(i)),
            WiFi.channel(i), WiFi.RSSI(i)
        );
      }
      WiFi.scanDelete();
    }
  });

  edgentConsole.addCommand("firmware", [](int argc, const char** argv) {
    if (argc < 1 || 0 == strcmp(argv[0], "info")) {
      unsigned sketchSize = ESP.getSketchSize();
      unsigned partSize = sketchSize + ESP.getFreeSketchSpace();

      edgentConsole.printf(" Version:   %s (build %s)\n", BLYNK_FIRMWARE_VERSION, __DATE__ " " __TIME__);
      edgentConsole.printf(" Type:      %s\n", BLYNK_FIRMWARE_TYPE);
      edgentConsole.printf(" Platform:  %s\n", BLYNK_INFO_DEVICE);
      edgentConsole.printf(" SDK:       %s\n", ESP.getSdkVersion());
      edgentConsole.printf(" ESP Core:  %s\n", ESP.getCoreVersion().c_str());

      edgentConsole.printf(" App size:  %dK (%d%%)\n", sketchSize/1024, (sketchSize*100)/partSize);
      edgentConsole.printf(" App MD5:   %s\n", ESP.getSketchMD5().c_str());

    }
  });

  edgentConsole.addCommand("status", [](int argc, const char** argv) {
    const uint64_t t = micros64() / 1000000;
    unsigned secs = t % BLYNK_SECS_PER_MIN;
    unsigned mins = (t / BLYNK_SECS_PER_MIN) % BLYNK_SECS_PER_MIN;
    unsigned hrs  = (t % BLYNK_SECS_PER_DAY) / BLYNK_SECS_PER_HOUR;
    unsigned days = t / BLYNK_SECS_PER_DAY;

    uint32_t heap_free; uint16_t heap_max;
    uint8_t heap_frag;
    ESP.getHeapStats(&heap_free, &heap_max, &heap_frag);
    edgentConsole.printf(" Uptime:          %dd %dh %dm %ds\n", days, hrs, mins, secs);
    edgentConsole.printf(" Reset reason:    %s\n",        ESP.getResetReason().c_str());
    edgentConsole.printf(" Flash:           %dK\n",       ESP.getFlashChipSize() / 1024);
    edgentConsole.printf(" Stack unused:    %d\n",        ESP.getFreeContStack());
    edgentConsole.printf(" Heap free:       %d / %d\n",   heap_free, heap_max);
    edgentConsole.printf("      fragment:   %d\n",        heap_frag);
    edgentConsole.printf("      max alloc:  %d\n",        ESP.getMaxFreeBlockSize());
#ifdef BLYNK_FS
    FSInfo fs_info;
    BLYNK_FS.info(fs_info);
    edgentConsole.printf(" FS free:         %d / %d\n",   (fs_info.totalBytes-fs_info.usedBytes), fs_info.totalBytes);
#endif
  });

#ifdef BLYNK_FS

  edgentConsole.addCommand("ls", [](int argc, const char** argv) {
    const char* path = (argc < 1) ? "/" : argv[0];
    Dir dir = BLYNK_FS.openDir(path);
    while (dir.next()) {
      File f = dir.openFile(BLYNK_FILE_READ);

      MD5Builder md5;
      md5.begin();
      md5.addStream(f, f.size());
      md5.calculate();
      String md5str = md5.toString();

      edgentConsole.printf("%8d %-24s %s\n",
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

