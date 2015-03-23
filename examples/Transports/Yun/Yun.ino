#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <Bridge.h>
#include <BlynkSimpleYun.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);
  // Or specify server using one of those commands:
  //Blynk.begin(auth, "server.org", 8080);
  //Blynk.begin(auth, server_ip, port);
}

void loop()
{
  Blynk.run();
}

