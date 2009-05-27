#include <stdlib.h>
#include <stdarg.h>
#include <uart.h>

#include "apDefinitions.h"
#include "simpleUart.h"

// FIXME: just grabbing MIDG_UBRG
#include "midg.h"

// UART and Buffer initialization
void uartInit (void){
	// Configure and open the port;
	// U1MODE Register
	// ==============
	U1MODEbits.UARTEN	= 0;		// Disable the port		
	U1MODEbits.USIDL 	= 0;		// Stop on idle
	U1MODEbits.IREN		= 0;		// No IR decoder
	U1MODEbits.RTSMD	= 0;		// Ready to send mode (irrelevant)
	U1MODEbits.UEN		= 0;		// Only RX and TX
	U1MODEbits.WAKE		= 1;		// Enable at startup
	U1MODEbits.LPBACK	= 0;		// Disable loopback
	U1MODEbits.ABAUD	= 0;		// Disable autobaud
	U1MODEbits.URXINV	= 0;		// Normal operation (high is idle)
	U1MODEbits.PDSEL	= 0;		// No parity 8 bit
	U1MODEbits.STSEL	= 0;		// 1 stop bit
	U1MODEbits.BRGH 	= 0;		// Low speed mode
	
	// U1STA Register
	// ==============
	//U1STAbits.URXISEL	= 2;		// RX interrupt when 3 chars are in
    U1STAbits.URXISEL	= 0;		// RX interrupt when just one char comes in
	U1STAbits.OERR		= 0;		// clear overun error
	
	// U1BRG Register
	// ==============
	//U1BRG = MIDG_UBRG;			// Set the baud rate to try and keep up with MIDG
    U1BRG = UCSCAP_UBRGF; // 19200
    
	// Enable the port;
	U1MODEbits.UARTEN	= 1;		// Enable the port	
	U1STAbits.UTXEN		= 1;		// Enable TX
	
	// wait for the UART to settle
	int i;
	for( i = 0; i < 32700; i += 1 )
	{
		Nop();
	}
    
    printToUart1("uart1 initialized in uartInit()\n\r");
    while(BusyUART1());

    printToUart1("uart1 still working in uartInit()?\n\r");
    while(BusyUART1());		
}

void printToUart2 (const char *fmt, ...){
	va_list ap;
	char buf [300];
	
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end (ap);
	putsUART2((unsigned int*)buf);
    while(BusyUART2());	
}

void printToUart1 (const char *fmt, ...){
	va_list ap;
	char buf [300];
	
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end (ap);
	putsUART1((unsigned int*)buf);
    while(BusyUART1());	
}
