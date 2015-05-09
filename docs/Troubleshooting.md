# Troubleshooting

### Delay

    void loop()
    {
      delay(1000);
      Blynk.run();
    }

### Flood

    void loop()
    {
      Blynk.virtualWrite(1, value);
      Blynk.run();
    }

### Serial connection
