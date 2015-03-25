/**
 * @file       main.cpp
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 */

//#define BLYNK_DEBUG
#define BLYNK_PRINT stdout
#ifdef RASPBERRY
 #include <BlynkApiWiringPi.h>
#else
 #include <BlynkApiLinux.h>
#endif
#include <BlynkSocket.h>
#include <getopt.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);

bool parse_options(int argc, char* argv[],
                   const char*& auth,
                   const char*& serv,
                   const char*& port)
{
    static struct option long_options[] = {
        {"token",   required_argument,   0, 't'},
        {"server",  required_argument,   0, 's'},
        {"port",    required_argument,   0, 'p'},
        {0, 0, 0, 0}
    };

    // Set default values
    auth = NULL;
    serv = BLYNK_DEFAULT_DOMAIN;
    port = STRINGIFY(BLYNK_DEFAULT_PORT);

    int rez;
    while (-1 != (rez = getopt_long(argc, argv,"t:s:p:", long_options, NULL))) {
        switch (rez) {
        case 't': auth = optarg; break;
        case 's': serv = optarg; break;
        case 'p': port = optarg; break;
        default : return false;
        };
    };

    // Check mandatory options
    if (!auth) {
        return false;
    }

    return true;
}

void print_usage()
{

}


/*
 * Main function
 */

BLYNK_WRITE(1)
{
  BLYNK_LOG("Got a value: %s", param[0].asStr());
}

int main(int argc, char* argv[])
{
    const char *auth, *serv, *port;
    if (!parse_options(argc, argv, auth, serv, port)) {
        print_usage();
        return 1;
    }

    Blynk.begin(auth, serv, port);

    while(true) {
        Blynk.run();
    }

    return 0;
}
