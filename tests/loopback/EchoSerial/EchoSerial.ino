void setup(void)
{
  Serial1.begin(9600);
}

void draw(char c, int qty = 1) {
  static int col = 0;
  while (qty-- > 0) {
    Serial.print(c);
    col = (col + 1) % 80;
    if (!col) {
      Serial.println();
    }
  }
}

void loop(void)
{
  char buf[128];
  int qty = 0;
  while (Serial1.available()) {
    buf[qty] = Serial.read();
    draw(buf[qty]);
    qty++;
  }
  for (int i = 0; i < qty; ++i) {
    Serial1.write(buf[i]);
    draw('.');
  }
}

