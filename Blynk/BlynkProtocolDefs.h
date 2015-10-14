/**
 * @file       BlynkProtocolDefs.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Blynk protocol definitions
 *
 */

#ifndef BlynkProtocolDefs_h
#define BlynkProtocolDefs_h

enum BlynkCmd
{
    BLYNK_CMD_RESPONSE       = 0,
    BLYNK_CMD_REGISTER       = 1,
    BLYNK_CMD_LOGIN          = 2,
    BLYNK_CMD_SAVE_PROF      = 3,
    BLYNK_CMD_LOAD_PROF      = 4,
    BLYNK_CMD_GET_TOKEN      = 5,
    BLYNK_CMD_PING           = 6,
    BLYNK_CMD_ACTIVATE       = 7,
    BLYNK_CMD_DEACTIVATE     = 8,
    BLYNK_CMD_REFRESH        = 9,
    BLYNK_CMD_TWEET          = 12,
    BLYNK_CMD_EMAIL          = 13,
    BLYNK_CMD_NOTIFY         = 14,
    BLYNK_CMD_BRIDGE         = 15,
    BLYNK_CMD_HARDWARE       = 20
};

enum BlynkStatus
{
    BLYNK_SUCCESS                = 200,
    BLYNK_QUOTA_LIMIT_EXCEPTION  = 1,
    BLYNK_ILLEGAL_COMMAND        = 2,
    BLYNK_NOT_REGISTERED         = 3,
    BLYNK_ALREADY_REGISTERED     = 4,
    BLYNK_NOT_AUTHENTICATED      = 5,
    BLYNK_NOT_ALLOWED            = 6,
    BLYNK_NO_CONNECTION          = 7,
    BLYNK_NO_ACTIVE_DASHBOARD    = 8,
    BLYNK_INVALID_TOKEN          = 9,
    BLYNK_DEVICE_WENT_OFFLINE    = 10,
    BLYNK_ALREADY_LOGGED_IN      = 11,

    BLYNK_NTF_INVALID_BODY       = 13,
    BLYNK_NTF_NOT_AUTHORIZED     = 14,
    BLYNK_NTF_ECXEPTION          = 15,

    BLYNK_TIMEOUT                = 16
};

struct BlynkHeader
{
    uint8_t  type;
    uint16_t msg_id;
    uint16_t length;
}
BLYNK_ATTR_PACKED;

#if defined(ARDUINO) || defined(ESP8266) || defined(PARTICLE) || defined(MBED_LIBRARY_VERSION)
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

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define BLYNK_HW_IN (('i' << 0) | ('n' << 8))
    #define BLYNK_HW_PM (('p' << 0) | ('m' << 8))
	#define BLYNK_HW_DW (('d' << 0) | ('w' << 8))
	#define BLYNK_HW_DR (('d' << 0) | ('r' << 8))
	#define BLYNK_HW_AW (('a' << 0) | ('w' << 8))
	#define BLYNK_HW_AR (('a' << 0) | ('r' << 8))
	#define BLYNK_HW_VW (('v' << 0) | ('w' << 8))
	#define BLYNK_HW_VR (('v' << 0) | ('r' << 8))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define BLYNK_HW_IN (('i' << 8) | ('n' << 0))
	#define BLYNK_HW_PM (('p' << 8) | ('m' << 0))
	#define BLYNK_HW_DW (('d' << 8) | ('w' << 0))
	#define BLYNK_HW_DR (('d' << 8) | ('r' << 0))
	#define BLYNK_HW_AW (('a' << 8) | ('w' << 0))
	#define BLYNK_HW_AR (('a' << 8) | ('r' << 0))
	#define BLYNK_HW_VW (('v' << 8) | ('w' << 0))
	#define BLYNK_HW_VR (('v' << 8) | ('r' << 0))
#else
    #error byte order problem
#endif

#endif
