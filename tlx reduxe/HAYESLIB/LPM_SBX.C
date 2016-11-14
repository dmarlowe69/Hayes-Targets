/* file: lpm_sbx.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("LPM-SBX PPI Initialization")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *	LPM-SBX PPI Initialization functions
 *
 *	written by denton marlowe
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
 *	Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<conio.h>

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"
 #include	"8255.h"
 #include	"lpm_sbx.h"

 #ifdef REMOTE_I_O

 #include	"remoteio.h"

 #endif

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 struct PPI lpm_sbx[4];

/****************************************************************************
 *
 *	PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_lpm_sbx(int port_0, int port_1, int port_2, int port_3);
						/* initialize ppi on LBX-PIO CARD */
 int toggle_bit_lpm_sbx(int bit, int port);	/* toggle PPI port c bits */
 int test_bit_lpm_sbx(int bit, int port);	/* test PPI port c bit */
 int set_bit_lpm_sbx(int bit, int port);	/* set PPI port c bit */
 int reset_bit_lpm_sbx(int bit, int port);	/* reset PPI port c bit */

 int read_porta_lpm_sbx(int port);		/* read byte from port A */
 int read_portb_lpm_sbx(int port);		/* read byte from port A */
 int read_portc_lpm_sbx(int port);		/* read byte from port A */

 int write_porta_lpm_sbx(int byte, int port);	/* write byte from port A */
 int write_portb_lpm_sbx(int byte, int port);	/* write byte from port A */
 int write_portc_lpm_sbx(int byte, int port);	/* write byte from port A */

/****************************************************************************
 *
 *	end of hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      PPi initialization
 *
 *	DELCARATION:	void init_ppi_lpm_sbx(int port_0, int port_1)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    ppi hardware initialized
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid,int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *	program 82c55A ppi on LPM-SBX card
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_lpm_sbx(int port_0, int port_1, int port_2, int port_3)
 {

	lpm_sbx[0].ppi_base_addr = mcs0_0_porta;
	lpm_sbx[0].port_a_addr = mcs0_0_porta;
	lpm_sbx[0].port_b_addr = mcs0_0_portb;
	lpm_sbx[0].port_c_addr = mcs0_0_portc;
	lpm_sbx[0].cntrl_addr = mcs0_0_control;

	lpm_sbx[1].ppi_base_addr = mcs1_0_porta;
	lpm_sbx[1].port_a_addr = mcs1_0_porta;
	lpm_sbx[1].port_b_addr = mcs1_0_portb;
	lpm_sbx[1].port_c_addr = mcs1_0_portc;
	lpm_sbx[1].cntrl_addr = mcs1_0_control;

	lpm_sbx[2].ppi_base_addr = mcs0_1_porta;
	lpm_sbx[2].port_a_addr = mcs0_1_porta;
	lpm_sbx[2].port_b_addr = mcs0_1_portb;
	lpm_sbx[2].port_c_addr = mcs0_1_portc;
	lpm_sbx[2].cntrl_addr = mcs0_1_control;

	lpm_sbx[3].ppi_base_addr = mcs1_1_porta;
	lpm_sbx[3].port_a_addr = mcs1_1_porta;
	lpm_sbx[3].port_b_addr = mcs1_1_portb;
	lpm_sbx[3].port_c_addr = mcs1_1_portc;
	lpm_sbx[3].cntrl_addr = mcs1_1_control;

	outp( mcs0_0_control, port_0 );
	outp( mcs1_0_control, port_1 );
	outp( mcs0_1_control, port_2 );
	outp( mcs1_1_control, port_3 );
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      toggle ppi port c bit
 *
 *	DELCARATION:	int toggle_bit_sbx(int bit, int port)
 *
 *	PARAMETERS:	ppi port c bit number (0..7)
 *			ppi port number (0 or 1)
 *
 *      RETURNS:        state of bit after toggle
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    ppi port c bit toggled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int inp(unsigned portid)
 *                      int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *      This function toggles the given bit of ppi port c, using the bit
 *      set/reset function of the device.
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int toggle_bit_lpm_sbx(int bit, int port)
 {
	if( (inp(lpm_sbx[port].port_c_addr) & (0x01<<bit)) == 0)
		{
		outp(lpm_sbx[port].cntrl_addr,(bit << 1) | 0x01);	/* set bit */
		return(BIT_SET);
		}
	else
		{
		outp(lpm_sbx[port].cntrl_addr,(bit << 1));	     /* reset bit */
		return(BIT_RESET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      test ppi port c bit
 *
 *	DELCARATION:	int test_bit_lpm_sbx(int bit, int port)
 *
 *      PARAMETERS:     ppi port c bit number (0..7)
 *			ppi port number (0 or 1)
 *
 *      RETURNS:        state of bit
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
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *      This function toggles the given bit of ppi port c, using the bit
 *      set/reset function of the device.
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int test_bit_lpm_sbx(int bit, int port)
 {
	if( (inp(lpm_sbx[port].port_c_addr) & (0x01<<bit)) == 0)
		{
		return(BIT_RESET);
		}
	else
		{
		return(BIT_SET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      set ppi port c bit
 *
 *	DELCARATION:	int set_bit_lpm_sbx(int bit, int port)
 *
 *      PARAMETERS:     ppi port c bit number (0..7)
 *			ppi port number (0 or 1)
 *
 *      RETURNS:        state of bit of changed, otherwise PORT_ERROR (-1)
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
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *      This function sets the given bit of ppi port c, using the bit
 *      set/reset function of the device.
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int set_bit_lpm_sbx(int bit, int port)
 {
	outp(lpm_sbx[port].cntrl_addr,(bit << 1) | 0x01);	/* set bit */

	if( (inp(lpm_sbx[port].port_c_addr) & (0x01<<bit)) != (0x01<<bit))
		return(PORT_ERROR);
	else
		return(BIT_SET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      reset ppi port c bit
 *
 *	DELCARATION:	int reset_bit_lpm_sbx(int bit, int port)
 *
 *      PARAMETERS:     ppi port c bit number (0..7)
 *			ppi port number (0 or 1)
 *
 *      RETURNS:        state of bit of changed, otherwise PORT_ERROR (-1)
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
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *      This function resets the given bit of ppi port c, using the bit
 *      set/reset function of the device.
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int reset_bit_lpm_sbx(int bit, int port)
 {
	outp(lpm_sbx[port].cntrl_addr,(bit << 1));	     /* reset bit */

	if( (inp(lpm_sbx[port].port_c_addr) & (0x01<<bit)) != 0)
		return(PORT_ERROR);
	else
		return(BIT_RESET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port a
 *
 *	DELCARATION:	int read_porta_lpm_sbx(int port)
 *
 *	PARAMETERS:	ppi port number (0 or 1)
 *
 *	RETURNS:	byte read from port
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
 *	 Referenced:	int inp(unsigned portid)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte form the ppi parallel port
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_porta_lpm_sbx(int port)
 {
 int byte;

	byte = inp(lpm_sbx[port].port_a_addr);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port b
 *
 *	DELCARATION:	int read_portb_lpm_sbx(int port)
 *
 *	PARAMETERS:	ppi port number (0 or 1)
 *
 *	RETURNS:	byte read from port
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
 *	 Referenced:	int inp(unsigned portid)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte form the ppi parallel port
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_portb_lpm_sbx(int port)
 {
 int byte;

	byte = inp(lpm_sbx[port].port_b_addr);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port c
 *
 *	DELCARATION:	int read_portc_lpm_sbx(int port)
 *
 *	PARAMETERS:	ppi port number (0 or 1)
 *
 *	RETURNS:	byte read from port
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
 *	 Referenced:	int inp(unsigned portid)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte form the ppi parallel port
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_portc_lpm_sbx(int port)
 {
 int byte;

	byte = inp(lpm_sbx[port].port_c_addr);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port a
 *
 *	DELCARATION:	int write_porta_lpm_sbx(int byte, int port)
 *
 *	PARAMETERS:	byte value
 *			ppi port number (0 or 1)
 *
 *	RETURNS:	byte written to port
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
 *	 Referenced:	int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *	This function writes a byte to the ppi parallel port
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int write_porta_lpm_sbx(int byte, int port)
 {
	outp(lpm_sbx[port].port_a_addr,byte);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port b
 *
 *	DELCARATION:	int write_portb_lpm_sbx(int byte, int port)
 *
 *	PARAMETERS:	byte value
 *			ppi port number (0 or 1)
 *
 *	RETURNS:	byte written to port
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
 *	 Referenced:	int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *	This function writes a byte to the ppi parallel port
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int write_portb_lpm_sbx(int byte, int port)
 {
	outp(lpm_sbx[port].port_b_addr,byte);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port c
 *
 *	DELCARATION:	int write_portc_lpm_sbx(int byte, int port)
 *
 *	PARAMETERS:	byte value
 *			ppi port number (0 or 1)
 *
 *	RETURNS:	byte written to port
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
 *	 Referenced:	int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *			8255.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lpm_sbx.c
 *
 *      DESCRIPTION:
 *
 *	This function writes a byte to the ppi parallel port
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int write_portc_lpm_sbx(int byte, int port)
 {
	outp(lpm_sbx[port].port_c_addr,byte);

	return(byte);
 }
