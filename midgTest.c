#include <stdlib.h>
#include <stdarg.h>
#include <uart.h>

#include "midg.h"
#include "simpleUart.h"
#include "midgTest.h"
#include "midgSplit.h"

void midgTestInit() {
    uartInit();
    
    midgInit();
    
    midgParserInit();
    
    // FIXME: the rest of this init routine can be removed...
    //        it's just for learning about the compiler/dsPic
    printToUart1("just a bit of testing...\n\r");
    
    printToUart1("test: sizeof(short) = %u\n\r", sizeof(short));
    printToUart1("test: sizeof(int) = %u\n\r", sizeof(int));
    printToUart1("test: sizeof(long) = %u\n\r", sizeof(long));
    
    tFloatToChar test;
    test.flData = 100.0; // Should be 0x42C80000
    printToUart1("100.0 should be 0x42C80000\n\r");
    printToUart1("%f = 0x%02X%02X%02X%02X\n\r", test.flData
                                                , test.chData[3]
                                                , test.chData[2]
                                                , test.chData[1]
                                                , test.chData[0] );
    printToUart1("dsPic is little endian?\n\r");
}

void copyMidgToUart1() {
    unsigned char buf[MIDG_CHUNKSIZE];
    int i;
    
    //printToUart1("copyMidgToUart1()\n\r");
    
    midgRead(buf);
    
    // UART testing
    printToUart1("%u:", buf[0]);
    for ( i = 1; i <= buf[0]; i++ ) {
        printToUart1("%02X", buf[i]);
        while(BusyUART1());
    }
    printToUart1(":%u\n\r", buf[MIDG_CHUNKSIZE-1]);
    //printToUart1("buf[MIDG_CHUNKSIZE-1] = %u\n\r",buf[MIDG_CHUNKSIZE-1]);
}

void midgParserToUart1() {
    unsigned char buf[MIDG_CHUNKSIZE];
    
    midgRead(buf);
    
    midgSplit(buf);
}
