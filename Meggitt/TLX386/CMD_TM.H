//////////////////////////////////////////////////////////////////////////////
//
//  data structures for cmd/tm  data link
//
//////////////////////////////////////////////////////////////////////////////

 #define NUM_CMD_TM_CHANNELS    16
 #define NUM_TM_CHANNELS        16
 #define NUM_CMD_CHANNELS       9

 #define TM_FRAME_COUNT         15

 struct CMD_TM {
	unsigned char addr;
	unsigned char bit[8];
	unsigned char num_channels;
	unsigned int frame_count;
	unsigned char data[NUM_CMD_TM_CHANNELS];
	};

//
//
//

 //extern volatile struct CMD_TM cmd;
  volatile struct CMD_TM cmd;

//
//
//

 //extern volatile struct CMD_TM tm;
 volatile struct CMD_TM tm;
