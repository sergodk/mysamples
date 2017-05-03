//---------------------------------------------------------------------------
#ifndef ambe3000_protocolH
#define ambe3000_protocolH

#pragma anon_unions

#include <stdbool.h>
#include "string.h"

typedef unsigned char BYTE;
						   	
#define MAXFRAMESIZE 255
#define MAXFIELDDATASIZE 255
#define MAXFIELDSINFRAME 10

#define TYPE_FRAME_CONTROL 0x00
#define TYPE_FRAME_CHANDATA 0x01

#define FIELD_TYPES_COUNT 31

//  Configuration Bytes Flags

// ===========================  CFG0 Flags  ===========================
#define CFG0_IF_SELECT0 1<<0
#define CFG0_IF_SELECT1 1<<1
#define CFG0_IF_SELECT2 1<<2
#define CFG0_DTX_ENABLE 1<<3
#define CFG0_SK_ENABLE 1<<4
#define CFG0_NS_ENABLE 1<<5
#define CFG0_CP_ENABLE 1<<6
#define CFG0_CP_SELECT 1<<7

//Codec Mode
#define INTERFACE_CODEC_SPI_CHANNEL_UART 0

// ===========================  CFG1 Flags  ===========================
#define CFG1_RATE0 1<<0
#define CFG1_RATE1 1<<1
#define CFG1_RATE2 1<<2
#define CFG1_RATE3 1<<3
#define CFG1_RATE4 1<<4
#define CFG1_RATE5 1<<5
#define CFG1_EC_ENABLE 1<<6
#define CFG1_ES_ENABLE 1<<7

//Vocoder Rates
#define VOCODER_RATE_2000_NOFEC CFG1_RATE0|CFG1_RATE1|CFG1_RATE2|CFG1_RATE3|CFG1_RATE4
#define VOCODER_RATE_2400_NOFEC_1 0
#define VOCODER_RATE_2400_NOFEC_2 CFG1_RATE0|CFG1_RATE2|CFG1_RATE5
#define VOCODER_RATE_2400_FEC50 CFG1_RATE0|CFG1_RATE2

// ===========================  CFG2 Flags  ===========================
#define CFG2_S_COM_RATE0 1<<0
#define CFG2_S_COM_RATE1 1<<1
#define CFG2_S_COM_RATE2 1<<2
#define CFG2_PATITY_ENABLE 1<<4

//Vocoder UART Baudrates
#define VOCODER_UART_BR_28800  0
#define VOCODER_UART_BR_57600  CFG2_S_COM_RATE0
#define VOCODER_UART_BR_115200 CFG2_S_COM_RATE1
#define VOCODER_UART_BR_230400 CFG2_S_COM_RATE0|CFG2_S_COM_RATE1
#define VOCODER_UART_BR_460800 CFG2_S_COM_RATE2

//---------------------------------------------------------------------------
typedef enum {
PKT_CHANNEL0=0,
PKT_ECMODE=1,
PKT_DCMODE=2,
PKT_COMPAND=3,
PKT_RATET=4,
PKT_RATEP=5,
PKT_INIT=6,
PKT_LOWPOWER=7,
PKT_CODECCFG=8,
PKT_CODECSTART=9,
PKT_CODECSTOP=10,
PKT_CHANFMT=11,
PKT_SPCHFMT=12,
PKT_PRODID=13,
PKT_VERSTRING=14,
PKT_READY=15,
PKT_HALT=16,
PKT_RESET=17,
PKT_RESETSOFTCFG=18,
PKT_GETCFG=19,
PKT_READCFG=20,
PKT_PARITYMODE=21,
PKT_WRITE_I2C=22,
PKT_CLRCODECRESET=23,
PKT_SETCODECRESET=24,
PKT_DISCARDCODEC=25,
PKT_DELAYNUS=26,
PKT_DELAYNNS=27,
PKT_RTSTHRESH=28,
PKT_GAIN=29,
PKT_UNKNOWN=30
} fieldType;
//---------------------------------------------------------------------------
static BYTE fieldCodes[FIELD_TYPES_COUNT] = {
0x40,      //   PKT_CHANNEL0
0x05,      //   PKT_ECMODE
0x06,      //   PKT_DCMODE
0x32,      //   PKT_COMPAND
0x09,      //   PKT_RATET
0x0A,      //   PKT_RATEP
0x0B,      //   PKT_INIT
0x10,      //   PKT_LOWPOWER
0x38,      //   PKT_CODECCFG
0x2A,      //   PKT_CODECSTART
0x2B,      //   PKT_CODECSTOP
0x15,      //   PKT_CHANFMT
0x16,      //   PKT_SPCHFMT
0x30,      //   PKT_PRODID
0x31,      //   PKT_VERSTRING
0x39,      //   PKT_READY
0x35,      //   PKT_HALT
0x33,      //   PKT_RESET
0x34,      //   PKT_RESETSOFTCFG
0x36,      //   PKT_GETCFG
0x37,      //   PKT_READCFG
0x3F,      //   PKT_PARITYMODE
0x44,      //   PKT_WRITE_I2C
0x46,      //   PKT_CLRCODECRESET
0x47,      //   PKT_SETCODECRESET
0x48,      //   PKT_DISCARDCODEC
0x49,      //   PKT_DELAYNUS
0x4A,      //   PKT_DELAYNNS
0x4E,      //   PKT_RTSTHRESH
0x4B,      //   PKT_GAIN
0x00       //   PKT_UNKNOWN
};

static BYTE fieldDataLength[FIELD_TYPES_COUNT] = {
0,      //   PKT_CHANNEL0
2,      //   PKT_ECMODE
2,      //   PKT_DCMODE
1,      //   PKT_COMPAND
1,      //   PKT_RATET
12,     //   PKT_RATEP
1,      //   PKT_INIT
1,      //   PKT_LOWPOWER
0xFF,   //   PKT_CODECCFG (varies length)
1,      //   PKT_CODECSTART
0,      //   PKT_CODECSTOP
2,      //   PKT_CHANFMT
2,      //   PKT_SPCHFMT
0,      //   PKT_PRODID
0,      //   PKT_VERSTRING
0,      //   PKT_READY
0,      //   PKT_HALT
0,      //   PKT_RESET
6,      //   PKT_RESETSOFTCFG
0,      //   PKT_GETCFG
0,      //   PKT_READCFG
1,      //   PKT_PARITYMODE
0xFF,   //   PKT_WRITE_I2C (varies length)
0,      //   PKT_CLRCODECRESET
0,      //   PKT_SETCODECRESET
2,      //   PKT_DISCARDCODEC
2,      //   PKT_DELAYNUS
2,      //   PKT_DELAYNNS
5,      //   PKT_RTSTHRESH
2,      //   PKT_GAIN
0       //   PKT_UNKNOWN
};

static BYTE fieldResponseLength[FIELD_TYPES_COUNT] = {
0,      //   PKT_CHANNEL0
0,      //   PKT_ECMODE
0,      //   PKT_DCMODE
0,      //   PKT_COMPAND
0,      //   PKT_RATET
0,      //   PKT_RATEP
0,      //   PKT_INIT
0,      //   PKT_LOWPOWER
0,      //   PKT_CODECCFG
1,      //   PKT_CODECSTART
1,      //   PKT_CODECSTOP
0,      //   PKT_CHANFMT
0,      //   PKT_SPCHFMT
0xFF,   //   PKT_PRODID  (varies length)
0xFF,   //   PKT_VERSTRING 	(varies length)
0,      //   PKT_READY
0,      //   PKT_HALT
0,      //   PKT_RESET
0,      //   PKT_RESETSOFTCFG
3,      //   PKT_GETCFG
3,      //   PKT_READCFG
0,      //   PKT_PARITYMODE
0,      //   PKT_WRITE_I2C
0,      //   PKT_CLRCODECRESET
0,      //   PKT_SETCODECRESET
0,      //   PKT_DISCARDCODEC
0,      //   PKT_DELAYNUS
0,      //   PKT_DELAYNNS
0,      //   PKT_RTSTHRESH
0,      //   PKT_GAIN
0       //   PKT_UNKNOWN
};

static char* fieldNames[FIELD_TYPES_COUNT] = {
"PKT_CHANNEL0",      //   PKT_CHANNEL0
"PKT_ECMODE",      //   PKT_ECMODE
"PKT_DCMODE",      //   PKT_DCMODE
"PKT_COMPAND",      //   PKT_COMPAND
"PKT_RATET",      //   PKT_RATET
"PKT_RATEP",     //   PKT_RATEP
"PKT_INIT",      //   PKT_INIT
"PKT_LOWPOWER",      //   PKT_LOWPOWER
"PKT_CODECCFG",   //   PKT_CODECCFG (varies length)
"PKT_CODECSTART",      //   PKT_CODECSTART
"PKT_CODECSTOP",      //   PKT_CODECSTOP
"PKT_CHANFMT",      //   PKT_CHANFMT
"PKT_SPCHFMT",      //   PKT_SPCHFMT
"PKT_PRODID",      //   PKT_PRODID
"PKT_VERSTRING",      //   PKT_VERSTRING
"PKT_READY",      //   PKT_READY
"PKT_HALT",      //   PKT_HALT
"PKT_RESET",      //   PKT_RESET
"PKT_RESETSOFTCFG",      //   PKT_RESETSOFTCFG
"PKT_GETCFG",      //   PKT_GETCFG
"PKT_READCFG",      //   PKT_READCFG
"PKT_PARITYMODE",      //   PKT_PARITYMODE
"PKT_WRITE_I2C",   //   PKT_WRITE_I2C (varies length)
"PKT_CLRCODECRESET",      //   PKT_CLRCODECRESET
"PKT_SETCODECRESET",      //   PKT_SETCODECRESET
"PKT_DISCARDCODEC",      //   PKT_DISCARDCODEC
"PKT_DELAYNUS",      //   PKT_DELAYNUS
"PKT_DELAYNNS",      //   PKT_DELAYNNS
"PKT_RTSTHRESH",      //   PKT_RTSTHRESH
"PKT_GAIN",      //   PKT_GAIN
"PKT_UNKNOWN"       //   PKT_UNKNOWN
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


typedef struct {
BYTE data[MAXFRAMESIZE];//full data with frame header

bool crc_on;
int size;               //full data with frame header size
int fieldslength;         //fields data size (without frame header size)

} Ambe3000Frame;
//---------------------------------------------------------------------------
typedef struct {
fieldType type;

BYTE data[MAXFIELDDATASIZE];
int datasize;
} Ambe3000Field;

//---------------------------------------------------------------------------

BYTE CRC_XOR(BYTE *buf, int n);

extern void InitAmbe3000Frame(Ambe3000Frame *frm, bool crc_need,int frametype);
extern void AddFieldAmbe3000Frame(Ambe3000Frame *frm, int fieldtype,int fielddatasize,BYTE *fielddata);
extern bool FinalizeAmbe3000Frame(Ambe3000Frame *frm);
extern void SetBitInByte(BYTE *b, int nbit,bool v);
//---------------------------------------------------------------------------
extern unsigned char GetAmbe3000Frame(unsigned char *buffer,int bufsize, Ambe3000Frame *gfr);
extern bool ParseAmbe3000Frame(Ambe3000Frame *frm, Ambe3000Field fields[], int *nfields,unsigned char is_in_frame, unsigned char *channel_data); 
extern void CreateAmbe3000SpeechFrame (Ambe3000Frame *afr, BYTE sizebits,BYTE *speechdatabuf);
//---------------------------------------------------------------------------
#endif 
