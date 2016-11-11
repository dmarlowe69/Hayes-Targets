// file: tcu.h

//////////////////////////////////////////////////////////////////////////////
//
//      Timer/Counter Unit (TCU) header file
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      TCU data structure
//

 struct TCU_TABLE {
	unsigned int ticks;             /* timing loop counter */
	};

 extern struct TCU_TABLE tcu_table;

//
//      TCU function prototypes
//


// Setup TCU for Baud Rate Generation
 void init_timer(unsigned int baud_rate);

// Restore TCU to BIOS ISR
 void deinit_timer(void);

// Install pointer to TCU ISR
 void install_application(void (far *routine)());

//  Dummy ISR
 void far dummy_application(void);
