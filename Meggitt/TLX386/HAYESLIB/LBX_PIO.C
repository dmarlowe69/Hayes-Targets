/* file: lbx_pio.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("PPI Initialization functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      PPI Initialization functions
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
 #include	"8255.h"
 #include	"lbx_pio.h"

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
 *	assign data storage for structure
 *
 ***************************************************************************/

 struct PPI sbx[2];

/****************************************************************************
 *
 *      PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_ppi_sbx(int port_0, int port_1);
					/* initialize ppi on LBX-PIO CARD */
 int toggle_bit_sbx(int bit, int port);	/* toggle PPI port c bits */
 int test_bit_sbx(int bit, int port);	/* test PPI port c bit */
 int set_bit_sbx(int bit, int port);	/* set PPI port c bit */
 int reset_bit_sbx(int bit, int port);	/* reset PPI port c bit */

 int read_porta_sbx(int port);			/* read byte from port A */
 int read_portb_sbx(int port);			/* read byte from port A */
 int read_portc_sbx(int port);			/* read byte from port A */

 int write_porta_sbx(int byte, int port);	/* write byte from port A */
 int write_portb_sbx(int byte, int port);	/* write byte from port A */
 int write_portc_sbx(int byte, int port);	/* write byte from port A */

/****************************************************************************
 *
 *      end of hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      PPi initialization
 *
 *	DELCARATION:	void init_ppi_sbx(int port_0, int port_1)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *      program 82c55A ppi on LPM-SBC40 cpu board for digitial I/O
 *
 *      port a output
 *      port b output
 *      port c output
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_ppi_sbx(int port_0, int port_1)
 {
	sbx[0].ppi_base_addr = mcs0_porta;
	sbx[0].port_a_addr = mcs0_porta;
	sbx[0].port_b_addr = mcs0_portb;
	sbx[0].port_c_addr = mcs0_portc;
	sbx[0].cntrl_addr = mcs0_control;

	sbx[1].ppi_base_addr = mcs1_porta;
	sbx[1].port_a_addr = mcs1_porta;
	sbx[1].port_b_addr = mcs1_portb;
	sbx[1].port_c_addr = mcs1_portc;
	sbx[1].cntrl_addr = mcs1_control;

	outp( mcs0_control, port_0 );
	outp( mcs1_control, port_1 );
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *      This function toggles the given bit of ppi port c, using the bit
 *      set/reset function of the device.
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int toggle_bit_sbx(int bit, int port)
 {
	if( (inp(sbx[port].port_c_addr) & (0x01<<bit)) == 0)
		{
		outp(sbx[port].cntrl_addr,(bit << 1) | 0x01);	/* set bit */
		return(BIT_SET);
		}
	else
		{
		outp(sbx[port].cntrl_addr,(bit << 1));	     /* reset bit */
		return(BIT_RESET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      test ppi port c bit
 *
 *	DELCARATION:	int test_bit_sbx(int bit, int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *      This function toggles the given bit of ppi port c, using the bit
 *      set/reset function of the device.
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int test_bit_sbx(int bit, int port)
 {
	if( (inp(sbx[port].port_c_addr) & (0x01<<bit)) == 0)
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
 *	DELCARATION:	int set_bit_sbx(int bit, int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *      This function sets the given bit of ppi port c, using the bit
 *      set/reset function of the device.
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int set_bit_sbx(int bit, int port)
 {
	outp(sbx[port].cntrl_addr,(bit << 1) | 0x01);	/* set bit */

	if( (inp(sbx[port].port_c_addr) & (0x01<<bit)) != (0x01<<bit))
		return(PORT_ERROR);
	else
		return(BIT_SET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      reset ppi port c bit
 *
 *	DELCARATION:	int reset_bit_sbx(int bit, int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *      This function resets the given bit of ppi port c, using the bit
 *      set/reset function of the device.
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int reset_bit_sbx(int bit, int port)
 {
	outp(sbx[port].cntrl_addr,(bit << 1));	     /* reset bit */

	if( (inp(sbx[port].port_c_addr) & (0x01<<bit)) != 0)
		return(PORT_ERROR);
	else
		return(BIT_RESET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port a
 *
 *	DELCARATION:	int read_porta_sbx(int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte form the ppi parallel port
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_porta_sbx(int port)
 {
 int byte;

	byte = inp(sbx[port].port_a_addr);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port b
 *
 *	DELCARATION:	int read_portb_sbx(int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte form the ppi parallel port
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_portb_sbx(int port)
 {
 int byte;

	byte = inp(sbx[port].port_b_addr);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port c
 *
 *	DELCARATION:	int read_portc_sbx(int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte form the ppi parallel port
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_portc_sbx(int port)
 {
 int byte;

	byte = inp(sbx[port].port_c_addr);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port a
 *
 *	DELCARATION:	int write_porta_sbx(int byte, int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *	This function writes a byte to the ppi parallel port
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int write_porta_sbx(int byte, int port)
 {
	outp(sbx[port].port_a_addr,byte);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port b
 *
 *	DELCARATION:	int write_portb_sbx(int byte, int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *	This function writes a byte to the ppi parallel port
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int write_portb_sbx(int byte, int port)
 {
	outp(sbx[port].port_b_addr,byte);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port c
 *
 *	DELCARATION:	int write_portc_sbx(int byte, int port)
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
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	lbx_pio.c
 *
 *      DESCRIPTION:
 *
 *	This function writes a byte to the ppi parallel port
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int write_portc_sbx(int byte, int port)
 {
	outp(sbx[port].port_c_addr,byte);

	return(byte);
 }
