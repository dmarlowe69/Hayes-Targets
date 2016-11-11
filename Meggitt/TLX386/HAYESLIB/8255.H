/* file: 8255.h */

/***************************************************************************
 *
 *	8255 PPI header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/***************************************************************************
 *
 *	General Bit defintions
 *
 ***************************************************************************/

 #define BIT0	0x00
 #define BIT1	0x01
 #define BIT2	0x02
 #define BIT3	0x03
 #define BIT4	0x04
 #define BIT5	0x05
 #define BIT6	0x06
 #define BIT7	0x07

/***************************************************************************
 *
 *      82c55A PPI MODE 0 port configurations Control words
 *
 ***************************************************************************/

                              /* port i/o direction      A  CL CH B */
 #define ppi_mode_0_0 0x80    /* mode 0 control word #0  o  o  o  o */
 #define ppi_mode_0_1 0x81    /* mode 0 control word #1  o  i  o  o */
 #define ppi_mode_0_2 0x82    /* mode 0 control word #2  o  o  o  i */
 #define ppi_mode_0_3 0x83    /* mode 0 control word #3  o  i  o  i */
 #define ppi_mode_0_4 0x88    /* mode 0 control word #4  o  o  i  o */
 #define ppi_mode_0_5 0x89    /* mode 0 control word #5  o  i  i  o */
 #define ppi_mode_0_6 0x8a    /* mode 0 control word #6  o  o  i  i */
 #define ppi_mode_0_7 0x8b    /* mode 0 control word #7  o  i  i  i */
 #define ppi_mode_0_8 0x90    /* mode 0 control word #8  i  o  o  o */
 #define ppi_mode_0_9 0x91    /* mode 0 control word #9  i  i  o  o */
 #define ppi_mode_0_10 0x92   /* mode 0 control word #10 i  o  o  i */
 #define ppi_mode_0_11 0x93   /* mode 0 control word #11 i  i  o  i */
 #define ppi_mode_0_12 0x98   /* mode 0 control word #12 i  o  i  o */
 #define ppi_mode_0_13 0x99   /* mode 0 control word #13 i  i  i  o */
 #define ppi_mode_0_14 0x9a   /* mode 0 control word #14 i  o  i  i */
 #define ppi_mode_0_15 0x9b   /* mode 0 control word #15 i  i  i  i */

/***************************************************************************
 *
 *      82c55A PPI MODE 0 port configurations Control words
 *
 ***************************************************************************/

 #define PPI_MODE_0	 0x80	/* base for MODE 0 control code */

 #define PORTA_OUT	 0x00	/* port a output */
 #define PORTB_OUT	 0x00	/* port b output */
 #define PORTC_LOWER_OUT 0x00	/* port c lower nibble output */
 #define PORTC_UPPER_OUT 0x00	/* port c upper nibble output */

 #define PORTA_IN	 0x10	/* port a output */
 #define PORTC_UPPER_IN  0x08	/* port c upper nibble output */
 #define PORTB_IN	 0x02	/* port b output */
 #define PORTC_LOWER_IN  0x01	/* port c lower nibble output */

/****************************************************************************
 *
 *      PPI data structure
 *
 ***************************************************************************/

 #ifndef PPI_STRUCT

 struct PPI {
        unsigned int ppi_base_addr;         /* base I/O address of PPI */
        unsigned int port_a_addr;           /* I/O address of port a */
        unsigned int port_b_addr;           /* I/O address of port b */
        unsigned int port_c_addr;           /* I/O address of port c */
        unsigned int cntrl_addr;            /* I/O address of control port */
        unsigned char port_a_direction;     /* input or output status */
        unsigned char port_b_direction;     /* input or output status */
        unsigned char port_c_low_direction; /* input or output status */
        unsigned char port_c_hi_direction;  /* input or output status */
        unsigned char port_a;               /* current value of port a */
        unsigned char port_b;               /* current value of port b */
        unsigned char port_c;               /* current value of port c */
	unsigned char mode;                 /* current value of mode */
	unsigned int port_addr[4];          /* I/O address array */
    };

 #define PPI_STRUCT

 #endif

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define BIT_SET        1
 #define BIT_RESET      0

 #define PPI_INPUT      0
 #define PPI_OUTPUT     1

 #define PORT_ERROR     -1

 #define PORT_A         0
 #define PORT_B         1
 #define PORT_C         2
 #define PORT_CNTRL     3
