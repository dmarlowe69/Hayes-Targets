// file: AIOTCU.H

//////////////////////////////////////////////////////////////////////////////
//
//      AIO Timer/Counter Unit (TCU) header file
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      TCU function prototypes
//

 void init_aio_timer(void);
 void aio_timer(unsigned int baud_rate);
 void deinit_aio_timer(void);
