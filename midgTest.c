#include <stdlib.h>
#include <stdarg.h>
#include <uart.h>

#include "midg.h"
#include "simpleUart.h"
#include "midgTest.h"
#include "midgSplit.h"

void midgTestInit() {
    // initialize UART1 for test and debug output
    uartInit();
    
    // initialize the MIDG's UART and configure messages to send
    midgInit();
    
    // initialize buffers for midgParser
    midgParserInit();
}

/* A test function that copies data from midgRead() to the UART1 to verify
 * everything is working.  Also includes the bytes sent and bytes remaining.
 */
void copyMidgToUart1() {
    unsigned char buf[MIDG_CHUNKSIZE];
    int i;
        
    midgRead(buf);
    
    // UART testing
    printToUart1("%u:", buf[0]);
    for ( i = 1; i <= buf[0]; i++ ) {
        printToUart1("%02X", buf[i]);
        while(BusyUART1());
    }
    printToUart1(":%u\n\r", buf[MIDG_CHUNKSIZE-1]);
}

/* This is an example of how the parser should be used, after initializing
 * as in midgTestInit()
 */
void midgParserToUart1() {
    unsigned char buf[MIDG_CHUNKSIZE];
    
    midgRead(buf);
    
    midgSplit(buf);
}
