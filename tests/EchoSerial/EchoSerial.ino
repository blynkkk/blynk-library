void setup(void)
{
  Serial.begin(9600);
}

void draw(char c) {
/*  Serial.print(c);
  static int col = 0;
  col = (col+1) % 80;
  if (!col) {
    Serial.println();
  }*/
}

void loop(void)
{
  char buf[128];
  int qty = 0;
  while (Serial.available()) {
    buf[qty] = Serial.read();
    draw(buf[qty]);
    qty++;
  }
  for (int i=0; i<qty; ++i) {
    Serial.write(buf[i]);
    draw('.');
  }
}

