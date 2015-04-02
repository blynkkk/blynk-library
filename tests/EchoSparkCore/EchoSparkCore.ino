
IPAddress server(192, 168, 0, 105);
uint16_t port = 8282;

void setup(void)
{
  Serial.begin(115200);

  delay(10000);
}

void draw(char c) {
  Serial.print(c);
  static int col = 0;
  col = (col+1) % 80;
  if (!col) {
    Serial.println(c);
  }
}

TCPClient client;

void loop(void)
{
  Serial.print("Connecting to ");
  Serial.print(server);  Serial.print(":");  Serial.println(port);

  if (1 == client.connect(server, port)) {
    Serial.println("Connected.");
    /* Echo incoming data */
    while (client.connected()) {
      char buf[128];
      int qty = 0;
      while (client.available() && qty < sizeof(buf)) {
        buf[qty] = client.read();
        draw(buf[qty]);
        qty++;
      }
      for (int i=0; i<qty; ++i) {
        client.write(buf[i]);
        draw('.');
      }
      Spark.process();
    }
  } else {
    Serial.println("Connection failed");
  }
  client.stop();
  // Delay before next connection attempt
  for (int i=0; i<50; i++) {
      Spark.process();
      delay(100);
  }
}

