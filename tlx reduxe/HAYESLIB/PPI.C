/* file: ppi.c */

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
 #include	"ppi.h"

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

 struct PPI ppi;

/****************************************************************************
 *
 *      PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_ppi(void);           /* initialize ppi on LPM-SBC40 card */
 int toggle_bit(int bit);       /* toggle PPI port c bits */
 int test_bit(int bit);         /* test PPI port c bit */
 int set_bit(int bit);          /* set PPI port c bit */
 int reset_bit(int bit);        /* reset PPI port c bit */

 int read_porta(void);          /* read byte from port A */
 int read_portb(void);          /* read byte from port A */
 int read_portc(void);          /* read byte from port A */

 int write_porta(int byte);     /* write byte from port A */
 int write_portb(int byte);     /* write byte from port A */
 int write_portc(int byte);     /* write byte from port A */

 int toggle_bit_port(int bit, int port);/* toggle PPI port bit */
 int test_bit_port(int bit, int port);  /* test PPI port bit */
 int set_bit_port(int bit, int port);   /* set PPI port bit */
 int reset_bit_port(int bit, int port); /* reset PPI port bit */

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
 *      DELCARATION:    void init_ppi(void)
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
 *      SOURCE:         ppi.c
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

 void init_ppi(void)
 {
	ppi.ppi_base_addr = ppi.port_addr[0] = ppi_porta;
	ppi.port_a_addr = ppi.port_addr[0] = ppi_porta;
	ppi.port_b_addr = ppi.port_addr[1] = ppi_portb;
	ppi.port_c_addr = ppi.port_addr[2] = ppi_portc;
	ppi.cntrl_addr = ppi.port_addr[3] = ppi_control;

	outp( ppi_control, ppi_mode_0_0 );	/* ports a, b and c outputs */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      toggle ppi port c bit
 *
 *      DELCARATION:    int toggle_bit(int bit)
 *
 *      PARAMETERS:     ppi port c bit number (0..7)
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
 *      SOURCE:         ppi.c
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

 int toggle_bit(int bit)
 {
	if( (inp(ppi_portc) & (0x01<<bit)) == 0)
		{
		outp(ppi_control,(bit << 1) | 0x01);    /* set bit */
		return(BIT_SET);
		}
	else
		{
		outp(ppi_control,(bit << 1));           /* reset bit */
		return(BIT_RESET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      test ppi port c bit
 *
 *      DELCARATION:    int test_bit(int bit)
 *
 *      PARAMETERS:     ppi port c bit number (0..7)
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
 *      SOURCE:         ppi.c
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

 int test_bit(int bit)
 {
	if( (inp(ppi_portc) & (0x01<<bit)) == 0)
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
 *      DELCARATION:    int set_bit(int bit)
 *
 *      PARAMETERS:     ppi port c bit number (0..7)
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
 *      SOURCE:         ppi.c
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

 int set_bit(int bit)
 {
	outp(ppi_control,(bit << 1) | 0x01);    /* set bit */

	if( (inp(ppi_portc) & (0x01<<bit)) != (0x01<<bit))
		return(PORT_ERROR);
	else
		return(BIT_SET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      reset ppi port c bit
 *
 *      DELCARATION:    int reset_bit(int bit)
 *
 *      PARAMETERS:     ppi port c bit number (0..7)
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
 *      SOURCE:         ppi.c
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

 int reset_bit(int bit)
 {
	outp(ppi_control,(bit << 1));           /* reset bit */

	if( (inp(ppi_portc) & (0x01<<bit)) != 0)
		return(PORT_ERROR);
	else
		return(BIT_RESET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port a
 *
 *      DELCARATION:    int read_porta(void)
 *
 *      PARAMETERS:     None
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
 *	SOURCE: 	ppi.c
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

 int read_porta(void)
 {
 int byte;

	byte = inp(ppi_porta);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port b
 *
 *      DELCARATION:    int read_portb(void)
 *
 *      PARAMETERS:     None
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
 *	SOURCE: 	ppi.c
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

 int read_portb(void)
 {
 int byte;

	byte = inp(ppi_portb);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read ppi port c
 *
 *      DELCARATION:    int read_portc(void)
 *
 *      PARAMETERS:     None
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
 *	SOURCE: 	ppi.c
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

 int read_portc(void)
 {
 int byte;

	byte = inp(ppi_portc);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port a
 *
 *      DELCARATION:    int write_porta(int byte)
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
 *	SOURCE: 	ppi.c
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

 int write_porta(int byte)
 {
	outp(ppi_porta,byte);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port b
 *
 *      DELCARATION:    int write_portb(int byte)
 *
 *	PARAMETERS:	byte value
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
 *	SOURCE: 	ppi.c
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

 int write_portb(int byte)
 {
	outp(ppi_portb,byte);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write a byte to ppi port c
 *
 *      DELCARATION:    int write_portc(int byte)
 *
 *	PARAMETERS:	byte value
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
 *	SOURCE: 	ppi.c
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

 int write_portc(int byte)
 {
	outp(ppi_portc,byte);

	return(byte);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      toggle ppi port bit
 *
 *      DELCARATION:    int toggle_bit_port(int bit, int port)
 *
 *      PARAMETERS:     ppi port bit number (0..7)
 *                      ppi port 0,1 or 2 (A,B or C)
 *
 *      RETURNS:        state of bit after toggle
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    ppi port bit toggled
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
 *      SOURCE:         ppi.c
 *
 *      DESCRIPTION:
 *
 *      This function toggles the given bit of ppi port
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int toggle_bit_port(int bit, int port)
 {
 int byte;
 int byte_in;

	if((bit < 0) | (bit > 7))
		return(PORT_ERROR);

	if((port < 0) | (port > 3))
		return(PORT_ERROR);

	byte = 0x01 << bit;
	byte_in = inp(ppi.port_addr[port]);

	if( (byte_in & byte) == 0)
	      {
		outp(ppi.port_addr[port],(byte_in | byte));
		return(BIT_SET);
		}
	else
		{
		outp(ppi.port_addr[port],(byte_in & ~byte));
		return(BIT_RESET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      test ppi port bit
 *
 *      DELCARATION:    int test_bit_port(int bit, int port)
 *
 *      PARAMETERS:     ppi port bit number (0..7)
 *                      ppi port 0,1 or 2 (A,B or C)
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
 *      SOURCE:         ppi.c
 *
 *      DESCRIPTION:
 *
 *      This function toggles the given bit of ppi port
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int test_bit_port(int bit, int port)
 {
	if((bit < 0) | (bit > 7))
		return(PORT_ERROR);

	if((port < 0) | (port > 3))
		return(PORT_ERROR);

	if( (inp(ppi.port_addr[port]) & (0x01<<bit)) == 0)
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
 *      set ppi port bit
 *
 *      DELCARATION:    int set_bit_port(int bit, int port)
 *
 *      PARAMETERS:     ppi port bit number (0..7)
 *                      ppi port 0,1 or 2 (A,B or C)
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
 *      SOURCE:         ppi.c
 *
 *      DESCRIPTION:
 *
 *      This function sets the given bit of ppi port
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int set_bit_port(int bit, int port)
 {
 int byte;
 int byte_in;

	if((bit < 0) | (bit > 7))
		return(PORT_ERROR);

	if((port < 0) | (port > 3))
		return(PORT_ERROR);

	byte = 0x01 << bit;
	byte_in = inp(ppi.port_addr[port]);

	outp(ppi.port_addr[port],(byte_in | byte));

	if( (inp(ppi.port_addr[port]) & (0x01<<bit)) != (0x01<<bit))
		return(PORT_ERROR);
	else
		return(BIT_SET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      reset ppi port bit
 *
 *      DELCARATION:    int reset_bit_port(int bit, int port)
 *
 *      PARAMETERS:     ppi port bit number (0..7)
 *                      ppi port 0,1 or 2 (A,B or C)
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
 *      SOURCE:         ppi.c
 *
 *      DESCRIPTION:
 *
 *      This function resets the given bit of ppi port
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int reset_bit_port(int bit, int port)
 {
 int byte;
 int byte_in;

	if((bit < 0) | (bit > 7))
		return(PORT_ERROR);

	if((port < 0) | (port > 3))
		return(PORT_ERROR);

	byte = 0x01 << bit;
	byte_in = inp(ppi.port_addr[port]);

	outp(ppi.port_addr[port],(byte_in & ~byte));

	if( (inp(ppi.port_addr[port]) & (0x01<<bit)) != 0)
		return(PORT_ERROR);
	else
		return(BIT_RESET);
 }
