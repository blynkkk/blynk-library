#!/usr/bin/env node

# This is a DEMO. You can use it only for development and testing.
#
# If you would like to add these features to your product,
# please contact Blynk for Business:
#
#                    http://tiny.cc/BlynkB2B
#

var express = require('express');
var fs = require('fs');

var app = express();
var db = JSON.parse(fs.readFileSync('ota-firmwares.json', 'utf8'));

app.get('/', function (req, res) {
  res.send('Simple ESP8266 OTA server');
});

app.get('/firmware.bin', function (req, res) {
  var auth = req.query.auth;
  //console.log(req.headers);

  if (req.get('user-agent') != 'ESP8266-http-Update') {
    res.status(403).send('Only for ESP8266 updater!');
    return;
  }

  var esp_info = {
      chip_size:   req.get('x-esp8266-chip-size'),
      free_size:   req.get('x-esp8266-free-space'),
      sketch_size: req.get('x-esp8266-sketch-size'),
      version:     req.get('x-esp8266-version'),
      sdk_version: req.get('x-esp8266-sdk-version'),
      sta_mac:     req.get('x-esp8266-sta-mac'),
      ap_mac:      req.get('x-esp8266-ap-mac'),
      mode:        req.get('x-esp8266-mode'),
  };
  console.log('Auth:', auth)
  console.log('Info:', JSON.stringify(esp_info));
  var fw = db[auth];

  if (fw.version == esp_info.version) {
    res.status(304).send('No update is required');
    return;
  }

  console.log("Sending firmware update...");
  res.sendFile(fw.file, { root: __dirname });
});

app.listen(3000, function () {
  console.log('Firmware server listening on port 3000!');
});
