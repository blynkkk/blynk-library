/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/

enum State {
  MODE_WAIT_CONFIG,
  MODE_CONFIGURING,
  MODE_CONNECTING_NET,
  MODE_CONNECTING_CLOUD,
  MODE_RUNNING,
  MODE_OTA_UPGRADE,
  MODE_ERROR,

  MODE_MAX_VALUE
};

#if defined(APP_DEBUG)
const char* StateStr[MODE_MAX_VALUE] = {
  "WAIT_CONFIG",
  "CONFIGURING",
  "CONNECTING_NET",
  "CONNECTING_CLOUD",
  "RUNNING",
  "OTA_UPGRADE",
  "ERROR"
};
#endif

namespace BlynkState
{
  volatile State state;

  State get()        { return state; }
  bool  is (State m) { return (state == m); }
  void  set(State m);
};

