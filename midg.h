#ifndef _MIDG_H_
#define _MIDG_H_

// select which UART to use by setting exactly 
// one of the below to 1, the other(s) to 0
#define _MIDG_UART1_ 0
#define _MIDG_UART2_ 1

#define MIDGBAUD 115200
#define MIDG_UBRG 21 
// if FCY = 40 000 000

#define MIDG_CHUNKSIZE 200

/*******************************************************************************
 * Creates a new MIDG message with incorrect checksum bytes.
 *
 * NOTE: A call to midgMsgAppendChecksum() MUST be made to correctly fill the
 *       checksum bytes.
 *
 * Generic MIDG Message Format:
 * {SYNC 0, SYNC 1, ID, COUNT = N, PAYLOAD 1, ..., PAYLOAD N, CKSUM 0, CKSUM 1}
 *
 */

#define MIDG_MSG(id,count, ...) {129,161, id, count, __VA_ARGS__, 0xFF, 0xFF}

#define MIDG_CFG_SET(count, ...) MIDG_MSG(35,count,__VA_ARGS__)

#define MIDG_MSG_DIV(id,div) MIDG_CFG_SET(3,5,id,div)

#define MIDG_MSG_OFF(id) MIDG_MSG_DIV(id,0)

#define MIDG_RESET MIDG_MSG( 99, 4, 0x01, 0x31, 0x06, 0x55 )

/******************************************************************************/

void midgInit(void);
void midgConfig(void);
void midgMsgAppendChecksum(unsigned char* message);
void midgRead(unsigned char* midgChunk);


#endif /* _MIDG_H_ */
