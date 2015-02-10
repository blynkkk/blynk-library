#ifndef BlynkProtocolDefs_h
#define BlynkProtocolDefs_h

#include <inttypes.h>

#define BLYNK_ATTR_PACKED __attribute__ ((__packed__))

enum BlynkCmd
{
    BLYNK_CMD_RESPONSE			= 0,
    BLYNK_CMD_REGISTER			= 1,
    BLYNK_CMD_LOGIN				= 2,
    BLYNK_CMD_SAVE_PROF			= 3,
    BLYNK_CMD_LOAD_PROF			= 4,
    BLYNK_CMD_GET_TOKEN			= 5,
    BLYNK_CMD_PING				= 6,
    BLYNK_CMD_HARDWARE			= 20,
};

enum BlynkStatus
{
    BLYNK_SUCCESS				= 200,
    BLYNK_BAD_FORMAT			= 2,
    BLYNK_NOT_REGISTERED		= 3,
    BLYNK_ALREADY_REGISTERED	= 4,
    BLYNK_NO_LOGIN				= 5,
    BLYNK_NOT_ALLOWED			= 6,
    BLYNK_NO_CONNECTION			= 7,
    BLYNK_NOT_SUPPORTED			= 8,
    BLYNK_INVALID_TOKEN			= 9,
    BLYNK_SERVER_ERROR			= 10,	//! \todo What's the difference with 500?
    BLYNK_ALREADY_LOGGED_IN		= 11,
    BLYNK_SERVER_ERROR2			= 500,
};

struct BlynkHeader
{
    uint8_t  type;
    uint16_t msg_id;
    uint16_t length;
}
BLYNK_ATTR_PACKED;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )
    #define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                       ((x)<< 8 & 0x00FF0000UL) | \
                       ((x)>> 8 & 0x0000FF00UL) | \
                       ((x)>>24 & 0x000000FFUL) )
    #define ntohs(x) htons(x)
    #define ntohl(x) htonl(x)
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define htons(x) (x)
    #define htonl(x) (x)
    #define ntohs(x) (x)
    #define ntohl(x) (x)
#else
    #error byte order problem
#endif

#endif
