
#include <getopt.h>
#include <Blynk/BlynkDebug.h>

static
void parse_options(int argc, char* argv[],
                   const char*& auth,
                   const char*& serv,
                   uint16_t&    port)
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
    port = BLYNK_DEFAULT_PORT;

    const char* usage =
        "Usage: blynk [options]\n"
        "\n"
        "Options:\n"
        "  -t auth, --token=auth    Your auth token\n"
        "  -s addr, --server=addr   Server name (default: " BLYNK_DEFAULT_DOMAIN ")\n"
        "  -p num,  --port=num      Server port (default: " BLYNK_TOSTRING(BLYNK_DEFAULT_PORT) ")\n"
        "\n";

    int rez;
    while (-1 != (rez = getopt_long(argc, argv,"t:s:p:", long_options, NULL))) {
        switch (rez) {
        case 't': auth = optarg; break;
        case 's': serv = optarg; break;
        case 'p': port = atoi(optarg); break;
        default : printf(usage); exit(1);
        };
    };

    // Check mandatory options
    if (!auth) {
        printf(usage);
        exit(1);
    }
}
