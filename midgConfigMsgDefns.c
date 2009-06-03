// file meant to be included in midg.c, NOT compiled on its own
// must be included AFTER midg.h

static unsigned char midgMsg_Reset[] = MIDG_RESET;

// MIDG messages not enabled
static unsigned char midgMsgDiv_1[] = MIDG_MSG_OFF(1);
static unsigned char midgMsgDiv_2[] = MIDG_MSG_OFF(2);
static unsigned char midgMsgDiv_3[] = MIDG_MSG_OFF(3);
static unsigned char midgMsgDiv_15[] = MIDG_MSG_OFF(15);
static unsigned char midgMsgDiv_20[] = MIDG_MSG_OFF(20);
static unsigned char midgMsgDiv_21[] = MIDG_MSG_OFF(21);
static unsigned char midgMsgDiv_22[] = MIDG_MSG_OFF(22);
static unsigned char midgMsgDiv_23[] = MIDG_MSG_OFF(23);
static unsigned char midgMsgDiv_25[] = MIDG_MSG_OFF(25);
static unsigned char midgMsgDiv_26[] = MIDG_MSG_OFF(26);
static unsigned char midgMsgDiv_27[] = MIDG_MSG_OFF(27);


// Messages at 50 Hz
static unsigned char midgMsgDiv_10[] = MIDG_MSG_DIV(10,1);
static unsigned char midgMsgDiv_12[] = MIDG_MSG_DIV(12,1);

/*
// Messages at 5 Hz, just for testing
static unsigned char midgMsgDiv_10[] = MIDG_MSG_DIV(10,10);
static unsigned char midgMsgDiv_12[] = MIDG_MSG_DIV(12,10);
*/

static unsigned char* midgConfigMsgs[] = {   midgMsgDiv_1
                                            ,midgMsgDiv_2
                                            ,midgMsgDiv_3
                                            ,midgMsgDiv_10
                                            ,midgMsgDiv_12
                                            ,midgMsgDiv_15
                                            ,midgMsgDiv_20
                                            ,midgMsgDiv_21
                                            ,midgMsgDiv_22
                                            ,midgMsgDiv_23
                                            ,midgMsgDiv_25
                                            ,midgMsgDiv_26
                                            ,midgMsgDiv_27
                                            };

                                            
/*
static unsigned char* midgConfigMsgs[] = {   midgMsgDiv_10
                                            ,midgMsgDiv_12
                                            };
*/
                                            
