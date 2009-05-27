#ifndef _MIDG_H_
#define _MIDG_H_

#include "circBuffer.h"

#if __IN_DSPIC__
#include <p33fxxxx.h>
#else
#include <stdio.h>
#endif
/*
#include <uart.h>
*/

// select which UART to use by setting exactly 
// one of the below to 1, the other(s) to 0
#define _MIDG_UART1_ 0
#define _MIDG_UART2_ 1

#define MIDGBAUD 115200
#define MIDG_UBRG 21 
// if FCY = 40 000 000

// configuration messages

// Generic MIDG Message Format
// {SYNC 0, SYNC 1, ID, COUNT = N, PAYLOAD 1, ..., PAYLOAD N, CKSUM 0, CKSUM 1}

// CFG_SET
// {0x81,0xA1,35,COUNT,PAYLOAD,CKSUM0, CKSUM1}

/******************************************************************************
 * Creates a new MIDG message with incorrect checksum bytes.
 *
 * NOTE: A call to midgMsgAppendChecksum() MUST be made to correctly fill the
 *       checksum bytes.
 */
#define MIDG_MSG(id,count, ...) {129,161, id, count, __VA_ARGS__, 0xFF, 0xFF}

#define CFG_SET(count, ...) MIDG_MSG(35,count,__VA_ARGS__)

#define MSG_DIV(count, ...) CFG_SET(count,__VA_ARGS__)
/******************************************************************************/
void midgMsgAppendChecksum(unsigned char* message);

#define MIDG_CHUNKSIZE 100

void midgInit(void);
void midgRead(unsigned char* midgChunk);


#endif /* _MIDG_H_ */
