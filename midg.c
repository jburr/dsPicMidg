// Code by: John B. Burr, heavily based on bufferedUART.c
// Started 5/17/09

#include "apDefinitions.h"
#include "midg.h"
#include "circBuffer.h"

#if MIDG_CHUNKSIZE > 257
#error "MIDG_CHUNKSIZE must be defined to be <= 257 in midg.h"
#endif

#if __IN_DSPIC__
#warning "just a reminder: we're __IN_DSPIC__"
#include <p33fxxxx.h>
#include <uart.h>
#include "simpleUart.h"
#else
#warning "just a reminder: we're NOT __IN_DSPIC__ so we're compiling for a PC?"
#include <stdio.h>
#endif

// include all the configuration messages set up in midgMsgConfigDefn.c
#include "midgConfigMsgDefns.c"

// midg.c globals to be set up in init()
struct CircBuffer midgUartBufferData;  // struct should never be used directly!
CBRef midgUartBuffer;   // use the pointer!

// dsPic specific implementation goes in here; the rest can be either on PC or embeded
#if __IN_DSPIC__
    // pick UART1 or UART2 based on midg.h #defines
    #if ((_MIDG_UART1_ == 1) && (_MIDG_UART2_ == 0))
        #warning "just a reminder: midg.c using UART1"
        #define UMODEbits U1MODEbits
        #define USTAbits U1STAbits
        #define UBRG U1BRG
        #define URXIP IPC2bits.U1RXIP
        #define URXIF IFS0bits.U1RXIF
        #define URXIE IEC0bits.U1RXIE
        #define URXREG U1RXREG
        
        #define putcMidgUART WriteUART1
        #define BusyMidgUART BusyUART1
        #define printToMidgUart printToUart1
        
    #elif ((_MIDG_UART2_ == 1) && (_MIDG_UART1_ == 0))
        #warning "just a reminder: midg.c using UART2"
        #define UMODEbits U2MODEbits
        #define USTAbits U2STAbits
        #define UBRG U2BRG
        #define URXIP IPC7bits.U2RXIP
        #define URXIF IFS1bits.U2RXIF
        #define URXIE IEC1bits.U2RXIE
        #define URXREG U2RXREG

        #define putcMidgUART WriteUART2
        #define BusyMidgUART BusyUART2
        #define printToMidgUart printToUart2
    #else
        #error "Check midg.h for UART selection--exactly one must be selected"
    #endif

    #warning "just a reminder: using midgInit() for dsPic"
    
    // UART and Buffer initialization
    void midgInit (void){
        // initialize the circular buffer
        midgUartBuffer = (struct CircBuffer*)&midgUartBufferData;
        newCircBuffer(midgUartBuffer);
        
        // Configure and open the port;
        // UxMODE Register (set by #define in midg.h)
        // ==============
        UMODEbits.UARTEN	= 0;		// Disable the port		
        UMODEbits.USIDL 	= 0;		// Stop on idle
        UMODEbits.IREN		= 0;		// No IR decoder
        UMODEbits.RTSMD	    = 0;		// Ready to send mode (irrelevant)
        UMODEbits.UEN		= 0;		// Only RX and TX
        UMODEbits.WAKE		= 1;		// Enable at startup
        UMODEbits.LPBACK	= 0;		// Disable loopback
        UMODEbits.ABAUD	    = 0;		// Disable autobaud
        UMODEbits.URXINV	= 0;		// Normal operation (high is idle)
        UMODEbits.PDSEL	    = 0;		// No parity 8 bit
        UMODEbits.STSEL	    = 0;		// 1 stop bit
        UMODEbits.BRGH 	    = 0;		// Low speed mode
        
        // UxSTA Register (set by #define in midg.h)
        // ==============
        USTAbits.URXISEL	= 2;		// RX interrupt when 3 chars are in
        //USTAbits.URXISEL	= 0;		// RX interrupt when just one char comes in
        USTAbits.OERR		= 0;		// clear overun error
        
        // UxBRG Register (set by #define in midg.h)
        // ==============
        UBRG = MIDG_UBRG;			// Set the baud rate for MIDG's default
        
        // Enable the port;
        UMODEbits.UARTEN	= 1;		// Enable the port
        USTAbits.UTXEN		= 1;		// Enable TX
        
        // wait for the UART to settle
        int i;
        for( i = 0; i < 32700; i += 1 )
        {
            Nop();
        }
        
        // configure the MIDG to only send desired messages
        midgConfig();
        
        // Initialize the Interrupt  
        URXIP   = 6;    		// Interrupt priority 6  
        URXIF   = 0;    		// Clear the interrupt flag
        URXIE   = 1;    		// Enable interrupts
     
        // Enable the port;
        USTAbits.UTXEN		= 0;		// Disable TX	
        UMODEbits.UARTEN	= 1;		// Enable the port		
        
        // Clear the overrun error since MIDG has been 
        // transmitting this whole time all input is ignored once it is set
        USTAbits.OERR		= 0;		// clear overun error
        
    }
    
    // Interrupt service routine for MIDG UART port, selected in midg.h
    #if _MIDG_UART1_
    void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void){
    #elif _MIDG_UART2_
    void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void){
    #endif
        // Read the buffer while it has data
        // and add it to the circular buffer
        while(USTAbits.URXDA == 1){
            writeBack(midgUartBuffer, (unsigned char)URXREG);
        }
        
        // If there was an overun error clear it and continue
        if (USTAbits.OERR == 1){
            USTAbits.OERR = 0;
        }
        
        // clear the interrupt
        URXIF = 0;
    }
       
#else
    #warning "just a reminder: using midgInit() for PC"
    void midgInit (void){
        // initialize the circular buffer
        midgUartBuffer = newCircBuffer(BSIZE);
        
        midgConfig();
    }
          
#endif

void midgConfig() {
    int currentMessage;
    int currentByte;
    unsigned int numMessages = (unsigned int)(sizeof(midgConfigMsgs)/sizeof(unsigned char*));
    
    #if !__IN_DSPIC__
    printf("there are %u config messages\n", numMessages);
    #endif
    
    // iterate through the config messages
    for ( currentMessage = 0; currentMessage < numMessages; currentMessage++ ) {
        midgMsgAppendChecksum( midgConfigMsgs[currentMessage] );
        
        // send one byte out at a time.  midgConfigMsgs[currentMessage][3]+6 = count+6 = message length
        for ( currentByte = 0; currentByte < midgConfigMsgs[currentMessage][3]+6; currentByte++ ) {
            #if __IN_DSPIC__
            putcMidgUART(midgConfigMsgs[currentMessage][currentByte]);
            while(BusyMidgUART());
            #else 
            printf("%02X ", midgConfigMsgs[currentMessage][currentByte]);
            #endif
        }
        #if !__IN_DSPIC__
        printf("\n");
        #endif
        
        #if __IN_DSPIC__
        //maybe we'll have to wait here?
        int i;
        for( i = 0; i < 32700; i += 1 ) {
            Nop();
        }
        #endif
    }
    
    /*
    // lots of debug to the other UART
    for ( currentMessage = 0; currentMessage < numMessages; currentMessage++ ) {
        // send one byte out at a time.  midgConfigMsgs[currentMessage][3]+6 = count+6 = message length
        for ( currentByte = 0; currentByte < midgConfigMsgs[currentMessage][3]+6; currentByte++ ) {
            printToUart1("%02X ", midgConfigMsgs[currentMessage][currentByte]);
        }
        printToUart1("\n\r");
    }
     */
}

void midgRead(unsigned char* midgChunk) {
    unsigned int tmpLen = getLength(midgUartBuffer);
    unsigned int i;
    
    #if __IN_DSPIC__
    //printToUart1("tmpLen in midgRead() = %u\n\r", tmpLen);
    #else
    //printf("\n\ntmpLen in midgRead() = %u\n", tmpLen);
    #endif
    
    // midgChunk[0] = bytes in midgChunk after read
    // midgChunk[MIDG_CHUNKSIZE-1] = bytes remaining in buffer after read
    if ( tmpLen > MIDG_CHUNKSIZE - 2 ) {
        // read as much as we can after the front and last have been removed
        midgChunk[0] = MIDG_CHUNKSIZE - 2;  
        midgChunk[MIDG_CHUNKSIZE-1] = tmpLen - midgChunk[0];
    } else {
        midgChunk[0] = tmpLen;
        // no more bytes will be left in buffer after read (based on tmpLen)
        midgChunk[MIDG_CHUNKSIZE-1] = 0;
    }
    
    for ( i = 1; i <= midgChunk[0]; i++ ) {
        midgChunk[i] = readFront(midgUartBuffer);
    }
    
    #if __IN_DSPIC__
    //printToUart1("midgChunk[0] = %u\n\r",midgChunk[0]);
    //printToUart1("midgChunk[MIDG_CHUNKSIZE-1] = %u\n\r",midgChunk[MIDG_CHUNKSIZE-1]);
    #endif
}

void midgMsgAppendChecksum(unsigned char* message) {
    int checksum0;
    int checksum1;
    int n;
    
    int length = message[3]+6;

    /*
    #if !__IN_DSPIC__
    printf("in midgMsgAppendChecksum()\n");
    for ( n = 0; n < length; n++ ) {
       printf("%02X ", (unsigned int)message[n]);
    }
    printf("\n");
    #endif
     */
    
    for (checksum0=0, checksum1=0, n=2; n < length-2; n++) {
       checksum0=checksum0+message[n];
       checksum1=checksum1+checksum0;
    }
    
    message[length-2] = checksum0;
    message[length-1] = checksum1;
    
    /*
    #if !__IN_DSPIC__
    for ( n = 0; n < length; n++ ) {
       printf("%02X ", (unsigned int)message[n]);
    }
    printf("\n");
    #endif
     */
}
