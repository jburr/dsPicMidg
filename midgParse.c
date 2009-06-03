#include "midgParse.h"

#if __IN_DSPIC__
#include "simpleUart.h"
#endif

void midgParse(unsigned char * sentence) {
	static tMidgData midgControlData;
		
	if (getChecksum(sentence[3]+6, sentence)){
		switch (sentence[2]){
			case NAV_SENSOR:
				// timestamp
				midgControlData.timeStamp.chData[3] = sentence[4];
				midgControlData.timeStamp.chData[2] = sentence[5];
				midgControlData.timeStamp.chData[1] = sentence[6];
				midgControlData.timeStamp.chData[0] = sentence[7];
				// Rates                                       
				midgControlData.p.chData[1] 		= sentence[8];
				midgControlData.p.chData[0] 		= sentence[9];
				midgControlData.q.chData[1] 		= sentence[10];
				midgControlData.q.chData[0] 		= sentence[11];
				midgControlData.r.chData[1] 		= sentence[12];
				midgControlData.r.chData[0] 		= sentence[13];
				// Accels                                      
				midgControlData.ax.chData[1] 		= sentence[14];
				midgControlData.ax.chData[0] 		= sentence[15];
				midgControlData.ay.chData[1] 		= sentence[16];
				midgControlData.ay.chData[0] 		= sentence[17];
				midgControlData.az.chData[1] 		= sentence[18];
				midgControlData.az.chData[0] 		= sentence[19];
				// Euler                                       
				midgControlData.yaw.chData[1] 		= sentence[20];
				midgControlData.yaw.chData[0] 		= sentence[21];
				midgControlData.pitch.chData[1] 	= sentence[22];
				midgControlData.pitch.chData[0] 	= sentence[23];
				midgControlData.roll.chData[1] 		= sentence[24];
				midgControlData.roll.chData[0] 		= sentence[25];
				// Quaternions                                 
				midgControlData.qw.chData[3] 		= sentence[26];
				midgControlData.qw.chData[2] 		= sentence[27];
				midgControlData.qw.chData[1] 		= sentence[28];
				midgControlData.qw.chData[0] 		= sentence[29];
				midgControlData.qx.chData[3] 		= sentence[30];
				midgControlData.qx.chData[2] 		= sentence[31];
				midgControlData.qx.chData[1] 		= sentence[32];
				midgControlData.qx.chData[0] 		= sentence[33];
				midgControlData.qy.chData[3] 		= sentence[34];
				midgControlData.qy.chData[2] 		= sentence[35];
				midgControlData.qy.chData[1] 		= sentence[36];
				midgControlData.qy.chData[0] 		= sentence[37];
				midgControlData.qz.chData[3] 		= sentence[38];
				midgControlData.qz.chData[2] 		= sentence[39];
				midgControlData.qz.chData[1] 		= sentence[40];
				midgControlData.qz.chData[0] 		= sentence[41];
				// flags                                       
				midgControlData.flags 				= sentence[42];
				
                break;
            case NAV_PV:
			default:
                #if __IN_DSPIC__
                printToUart1("\n\runrecognized Message ID: %d", sentence[2]);
                #else
                printf("\n\runrecognized Message ID: %d", sentence[2]);
                #endif
		}
        // prints when any valid message comes in, even if midgControlData isn't updated
		printMidgData(midgControlData);	
	} else {
        #if __IN_DSPIC__
		printToUart1("\n\r=========== Checksum Failed!");
		printToUart1("\n\r Message ID: %d, Length: %d", sentence[2], sentence[3] );
        #else
		printf("\n\r=========== Checksum Failed!");
		printf("\n\r Message ID: %d, Length: %d", sentence[2], sentence[3] );
        #endif
	}

}

unsigned char getChecksum(unsigned char msgLen, unsigned char * msg){
	unsigned char checksum0 = 0, checksum1 = 0, n;
	
	for (n = 2; n < msgLen-2; n++) {
		checksum0 = checksum0 + msg[n];
		checksum1 = checksum1 + checksum0;
	}
	
	return (checksum0==msg[msgLen-2] && checksum1==msg[msgLen-1]);
}

void printMidgData (tMidgData midg2Print) {
	#if __IN_DSPIC__
    //printToUart1("\n\r__IN_DSPIC__:");
    
    // To match the MIDG II display: 
    // p,q,r in deg/sec; ax,ay,az in milli-g; yaw,pitch,roll in degrees
    // NOTE: This takes way too long at 50Hz, but is ok at 5Hz
    /*printToUart1("\n\r%lu\t%0.2f\t%0.2f\t%0.2f\t%d\t%d\t%d\t%0.2f\t%0.2f\t%0.2f", 
            midg2Print.timeStamp.ulData,
			midg2Print.p.shData*0.01,
			midg2Print.q.shData*0.01,
			midg2Print.r.shData*0.01,
            midg2Print.ax.shData,
            midg2Print.ay.shData,
            midg2Print.az.shData,
            midg2Print.yaw.shData*0.01,
            midg2Print.pitch.shData*0.01,
            midg2Print.roll.shData*0.01);
            */
    
    // Shorter printing of just accelerations in milli-g (less data, no floats)
    printToUart1("\n\r%lu\t%d\t%d\t%d", 
            midg2Print.timeStamp.ulData,
			midg2Print.ax.shData,
            midg2Print.ay.shData,
            midg2Print.az.shData );

    #else
    // To match the MIDG II display: 
    // p,q,r in deg/sec; ax,ay,az in milli-g; yaw,pitch,roll in degrees
    printf("\n\r%lu\t%0.2f\t%0.2f\t%0.2f\t%d\t%d\t%d\t%0.2f\t%0.2f\t%0.2f", 
            midg2Print.timeStamp.ulData,
			midg2Print.p.shData*0.01,
			midg2Print.q.shData*0.01,
			midg2Print.r.shData*0.01,
            midg2Print.ax.shData,
            midg2Print.ay.shData,
            midg2Print.az.shData,
            midg2Print.yaw.shData*0.01,
            midg2Print.pitch.shData*0.01,
            midg2Print.roll.shData*0.01);    
    #endif
}
