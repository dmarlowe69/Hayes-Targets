/* file: rlc.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("RLC Initialization functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *	RLC Initialization functions
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/90	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 5.0
 *	Microsoft C Version 7.00
 *	MASM Version 6.0
 *	Datalight C_Thru_ROM Version 2.0c
 *
 *              This Software is dedicated to the memory of
 *
 *                      NYSSA ANN MARLOWE
 *                  17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<conio.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"
 #include	"rlc.h"

 #ifdef REMOTE_I_O

 #include	"remoteio.h"

 #endif

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	RLC data structure
 *
 ***************************************************************************/

 #ifndef RLC_STRUCT

 struct RLC {
	unsigned int rlc_addr;		/* base I/O address of PPI */
	unsigned int relay_byte;
	unsigned int relay_read;
	unsigned int relay_write;
	};

 #define RLC_STRUCT

 #endif

 struct RLC rlc;			/* assign structure storage */

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define RELAY_SET	1
 #define RELAY_RESET	0

 #define RELAY_ERROR	-1

/****************************************************************************
 *
 *	RLC initialization function prototypes
 *
 ***************************************************************************/

 void init_rlc(void);		/* initialize RLC card */
 int toggle_relay(int relay);	/* toggle RLC relay */
 int test_relay(int relay);	/* test RLC relay */
 int set_relay(int relay);	/* set RLC relay */
 int reset_relay(int relay);	/* reset RLC relay */

 int write_relay(int relay);	/* write RLC control byte */
 int read_relay(void);		/* read RLC relay status */

/****************************************************************************
 *
 *      end of hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *	RLC initialization
 *
 *	DELCARATION:	void init_rlc(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *		Conditions:		rlc hardware initialized
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid,int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *						rlc.h
 *
 *      WARNINGS:       None
 *
 *		SOURCE: 		rlc.c
 *
 *      DESCRIPTION:
 *
 *	program RLC
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_rlc(void)
 {
	rlc.rlc_addr = RLC_BASE_ADDRESS;

	outp(rlc.rlc_addr,0);	/* turn off all relays */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	toggle rlc relay
 *
 *	DELCARATION:	int toggle_relay(int relay)
 *
 *	PARAMETERS:	rlc relay number (0..7)
 *
 *	RETURNS:	state of relay after toggle
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *	 Conditions:	rlc relay toggled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int inp(unsigned portid)
 *                      int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			rlc.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	rlc.c
 *
 *      DESCRIPTION:
 *
 *	This function toggles the given relay of rlc
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int toggle_relay(int relay)
 {

	if((relay < 0) | (relay > 7))
		return(RELAY_ERROR);

	rlc.relay_byte = 0x01 << relay;
	rlc.relay_read = inp(rlc.rlc_addr);

	if( (rlc.relay_read & rlc.relay_byte) ==0)
		{
		rlc.relay_write = rlc.relay_read | rlc.relay_byte;
		outp(rlc.rlc_addr,rlc.relay_write);
		return(RELAY_SET);
		}
	else
		{
		rlc.relay_write = rlc.relay_read & ~rlc.relay_byte;
		outp(rlc.rlc_addr,rlc.relay_write);
		return(RELAY_RESET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	test rlc
 *
 *	DELCARATION:	int test_relay(int relay)
 *
 *	PARAMETERS:	rlc relay number (0..7)
 *
 *	RETURNS:	state of relay
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int inp(unsigned portid)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			rlc.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	rlc.c
 *
 *      DESCRIPTION:
 *
 *	This function returns the state of the given relay of rlc.
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int test_relay(int relay)
 {
	if((relay < 0) | (relay > 7))
		return(RELAY_ERROR);

	rlc.relay_byte = 0x01 << relay;
	rlc.relay_read = inp(rlc.rlc_addr);

	if( (rlc.relay_read & rlc.relay_byte) ==0)
		{
		return(RELAY_RESET);
		}
	else
		{
		return(RELAY_SET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	set rlc relay
 *
 *	DELCARATION:	int set_relay(int relay)
 *
 *	PARAMETERS:	rlc relay number (0..7)
 *
 *	RETURNS:	state of relay of changed, otherwise PORT_ERROR (-1)
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			rlc.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	rlc.c
 *
 *      DESCRIPTION:
 *
 *	This function sets the given relay of rlc
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int set_relay(int relay)
 {
	if((relay < 0) | (relay > 7))
		return(RELAY_ERROR);

	rlc.relay_byte = 0x01 << relay;
	rlc.relay_read = inp(rlc.rlc_addr);

	rlc.relay_write = rlc.relay_read | rlc.relay_byte;
	outp(rlc.rlc_addr,rlc.relay_write);
	return(RELAY_SET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      reset rlc relay
 *
 *	DELCARATION:	int reset_relay(int relay)
 *
 *      PARAMETERS:     rlc relay number (0..7)
 *
 *	RETURNS:	state of relay of changed, otherwise PORT_ERROR (-1)
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			rlc.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	rlc.c
 *
 *      DESCRIPTION:
 *
 *      This function resets the given relay of rlc
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int reset_relay(int relay)
 {
	if((relay < 0) | (relay > 7))
		return(RELAY_ERROR);

	rlc.relay_byte = 0x01 << relay;
	rlc.relay_read = inp(rlc.rlc_addr);

	rlc.relay_write = rlc.relay_read & ~rlc.relay_byte;
	outp(rlc.rlc_addr,rlc.relay_write);
	return(RELAY_RESET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write relay
 *
 *	DELCARATION:	int write_relay(int relay)
 *
 *	PARAMETERS:	RLC relay byte
 *
 *	RETURNS:	state of relays updated, otherwise RELAY_ERROR (-1)
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			rlc.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	rlc.c
 *
 *      DESCRIPTION:
 *
 *	This function writes a byte to the relay card RLC
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int write_relay(int relay)
 {
	if((relay < 0) | (relay > 255))
		return(RELAY_ERROR);

	rlc.relay_byte = relay;
	outp(rlc.rlc_addr,relay);
	return(RELAY_SET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read relay
 *
 *	DELCARATION:	int relay_relay(void)
 *
 *	PARAMETERS:	None
 *
 *	RETURNS:	state of relays, otherwise RELAY_ERROR (-1)
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *	 Referenced:	int intp(unsigned portid)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			rlc.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	rlc.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte from the given relay card RLC
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_relay(void)
 {
 int relay;

	relay = inp(rlc.rlc_addr);
	rlc.relay_byte = relay;
	return(relay);
 }
