#include <stdio.h>
#include <stdlib.h>

#include "midgSim.h"
#include "midg.h"
#include "midgSplit.h"

int main(int argc, char** argv) {

	if ( argc != 2 ) {
		printf("usage: ./midgSimParserTest \"input file\"\n");
		exit(1);
	}
   
   midgInit();
   midgParserInit();
   
   long int size = midgSimGetInput( argv[1] );
   
   int i;
   int bytesRead = 0;
   unsigned char midgReadBuf[MIDG_CHUNKSIZE];
   while ( bytesRead < size ) {
      midgSimStep();
      
      midgRead(midgReadBuf);
      
	  midgSplit(midgReadBuf);

      bytesRead += midgReadBuf[0];
   }
   
   printf("%d bytes read.  now exiting.\n", bytesRead);
   exit(EXIT_SUCCESS);
}
