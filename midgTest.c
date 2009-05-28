#include <stdlib.h>
#include <stdarg.h>
#include <uart.h>

#include "midg.h"
#include "simpleUart.h"
#include "midgTest.h"

void midgTestInit() {
    uartInit();
    
    midgInit();
}

void copyMidgToUart1() {
    unsigned char buf[MIDG_CHUNKSIZE];
    int i;
    
    //printToUart1("copyMidgToUart1()\n\r");
    
    midgRead(buf);
    
    printToUart1("%u:", buf[0]);
    for ( i = 1; i <= buf[0]; i++ ) {
        printToUart1("%02X", buf[i]);
        while(BusyUART1());
    }
    printToUart1(":%u\n\r", buf[MIDG_CHUNKSIZE-1]);
    //printToUart1("buf[MIDG_CHUNKSIZE-1] = %u\n\r",buf[MIDG_CHUNKSIZE-1]);
    
}
