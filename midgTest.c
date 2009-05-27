#include <stdlib.h>
#include <stdarg.h>
#include <uart.h>

#include "midgTest.h"
#include "midg.h"
#include "circBuffer.h"

void copyMidgToUart1() {
    unsigned char buf[MIDG_CHUNKSIZE];
    int i;
    
    printToUart1("copyMidgToUart1()\n\r");
    
    /*
    // some extra test printing
    unsigned char test[] = MSG_DIV(2,50);
    midgMsgAppendChecksum( test );
    printToUart1("test of macros: ");
    for ( i = 0; i < 9; i++ ) {
        printToUart1("%u ", (unsigned int)test[i]);
    }
    printToUart1("... that was it.\n");
    */
    
    
    /*
    midgRead(buf);
    
    printToUart1("%u:<", buf[0]);
    for ( i = 1; i <= buf[0]; i++ ) {
        //putcUART1(buf[i]);
        printToUart1("%02X", buf[i]);
        while(BusyUART1());
    }
    printToUart1(">\r\n");
    */
}
