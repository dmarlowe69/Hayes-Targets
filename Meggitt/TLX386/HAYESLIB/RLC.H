/* file: rlc.h */

/***************************************************************************
 *
 *      RLC header file
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/91        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      RLC data structure
 *
 ***************************************************************************/

 #ifndef RLC_STRUCT

 struct RLC {
	unsigned int rlc_addr;          /* base I/O address of PPI */
	unsigned int relay_byte;
	unsigned int relay_read;
	unsigned int relay_write;
	};

 extern struct RLC rlc;

 #define RLC_STRUCT

 #endif

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define RELAY_SET      1
 #define RELAY_RESET    0

 #define RELAY_ERROR    -1

/****************************************************************************
 *
 *      RLC initialization function prototypes
 *
 ***************************************************************************/

 void init_rlc(void);           /* initialize RLC card */
 int toggle_relay(int relay);   /* toggle RLC relay */
 int test_relay(int relay);     /* test RLC relay */
 int set_relay(int relay);      /* set RLC relay */
 int reset_relay(int relay);    /* reset RLC relay */

 int write_relay(int relay);	/* write RLC control byte */
 int read_relay(void);		/* read RLC relay status */
