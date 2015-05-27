# Security

Blynk server has 3 ports open for different security levels.
* 8441 - SSL connection for hardware
* 8442 - TCP connection for hardware (no security)
* 8443 - 2WAY SSL connection for Mobile Apps

Hardware may select to connect to 8441 or 8442, depending on it's capabilities.

### SSL gateway

Most platforms are not capable to handle SSL, so they connect to 8442.
However, our [gateway script](https://github.com/blynkkk/blynk-library/blob/master/scripts/blynk-ser.sh) can be used to add SSL security layer to communication.

```bash
./blynk-ser.sh -f TCP
```
This will start TCP server on 8442 and forward each connection to the server via SSL.
You can run this script on your Raspberry Pi, desktop computer, or even directly on your router!

**Note:** when using your own server, you should overwrite the bundled server.crt certificate, or specify your sertificate to the script using --cert switch:

```bash
./blynk-ser.sh -f TCP -s <server ip> -p 8441 --cert=<certificate>.crt
```

Security is also enabled for USB communication forwarding by default.
If you want to skip SSL, and connect to TCP, you can also do that:

```bash
./blynk-ser.sh -t TCP
```

**Note:** SSL is supported by the gateway only on Linux/OSX for now
