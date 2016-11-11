/* file: platform.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Main function")
 #pragma subtitle("(c)1991-1993 Hayes Targets")

/****************************************************************************
 *
 *	MAIN function
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma message("AIO A/D CONFIGURATION")

 #define LPM_AIO_DC             /* winsystems analog input-output board */

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/91	D.marlowe	Original release
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

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	<hayes.h>
 #include	<gnd.h>
 #include   "config.h"
 #include   "pilot.h"

/****************************************************************************
 *
 *	BIT function prototypes
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Local Definitions
 *
 ***************************************************************************/

 #define TRUE 1

 void far brain(void);

 void a_d_config(void); 	/* configure A/D data structure */
 void d_a_config(void); 	/* configure D/A data structure */

 void init_brain(void);
 void far brain(void);
 void far house_keep(void);
 void init_systems(void);

/****************************************************************************
 *
 *	cmd/tm functions
 *
 ***************************************************************************/

 void init_cmd_tm_interface(void);
 void decoder(int on_off);
 void far data_cmd_application(void);
 void far data_cmd_irq(void);
 void far decode_cmd_data(void);

/*@H*************************************************************************
 *
 *	BRAIN initialize application function
 *
 *	written by denton marlowe
 *	(c)1990 Hayes Targets
 *
 ************************************************************************@H*/
 void init_brain(void)
 {
    init_systems();                         /* initialize hardware */

    autopilot_init();                       /* initialize autopliot */

	install_application(brain); 			/* activate autopilot */

    install_cmd_application(decode_cmd_data);
						/* activate command decoder */
 }
/****************************************************************************
 *
 *	Hardware Sample Rate Function
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void far brain(void)
 {
    if(tcu_table.ticks > 16) {
        tcu_table.ticks = 0;
        }

    autopilot();

 }

 #pragma page(1)

/****************************************************************************
 *
 *	House Keep application specific hardware
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void far house_keep(void)
 {

 }

 #pragma page(1)

/***************************************************************************
 *
 *	Inintialize application specific hardware
 *
 *	written by denton marlowe
 *	(c)1990 Hayes Targets
 *
 **************************************************************************/
 void init_systems(void)
 {

 }

 #pragma page(1)

/****************************************************************************
 *
 *	A/D Configuration Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void a_d_config(void)
 {
	int i;

 #ifdef LPM_AIO_DC

 #pragma message("AIO A/D CONFIGURATION")

    adc.ZERO = 0x0;             /* integer value for zero volts */
    adc.MAX = 5;                /* maximum input is +10 volts */
	adc.MIN = 0;				/* minimum input is zero volts */
	adc.GAIN = 1;				/* gain is set to 1 */
    adc.POLARITY = UNIPOLAR;    /* unipolar input */

 #elif def ARM

 #pragma message("ARM A/D CONFIGURATION")

	adc.ZERO = 0x000;			/* integer value for zero volts */
	adc.MAX = 10;				/* maximum input is +10 volts */
	adc.MIN = 0;				/* minimum input is zero volts */
	adc.GAIN = 1;				/* gain is set to 1 */
	adc.POLARITY = UNIPOLAR;	/* unipolar input */

 #else

 #pragma message("BIRD A/D CONFIGURATION")

	adc.ZERO = 0x800;			/* integer value for zero volts */
	adc.MAX = 10;				/* maximum input is +10 volts */
	adc.MIN = -10;				/* minimum input is zero volts */
	adc.GAIN = 1;				/* gain is set to 1 */
	adc.POLARITY = BIPOLAR;		/* unipolar input */

 #endif

	for(i=0;i<16;i++) {
		adc.A_D[i] = adc.ZERO;	/* zero input array */
		}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	D/A Configuration Routine
 *
 *	written by denton marlowe
 *	(c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void d_a_config(void)
 {

 #ifdef LPM_AIO_DC

 #pragma message("AIO A/D CONFIGURATION")

    dac[0].D_A = 0x0;               /* zero volts */
	dac[0].ADDR = D_A_CHANNEL_0;	/* address of D/A converter */
    dac[0].ZERO = 0x0;              /* integer value for zero volts */
    dac[0].MAX = 5;                 /* maximum output is +10 volts */
	dac[0].MIN = 0; 				/* minimum output is zero volts */
	dac[0].POLARITY = UNIPOLAR;		/* unipolar output */

    dac[1].D_A = 0x0;               /* zero volts */
	dac[1].ADDR = D_A_CHANNEL_1;	/* address of D/A converter */
    dac[1].ZERO = 0x0;              /* integer value for zero volts */
    dac[1].MAX = 5;                 /* maximum output is +10 volts */
	dac[1].MIN = 0; 				/* minimum output is zero volts */
    dac[1].POLARITY = UNIPOLAR;     /* unipolar output */

 #elif def ARM

 #pragma message("ARM D/A CONFIGURATION")

	dac[0].D_A = 0x000;				/* zero volts */
	dac[0].ADDR = D_A_CHANNEL_0;	/* address of D/A converter */
	dac[0].ZERO = 0x000;			/* integer value for zero volts */
	dac[0].MAX = 10;				/* maximum output is +10 volts */
	dac[0].MIN = 0; 				/* minimum output is zero volts */
	dac[0].POLARITY = UNIPOLAR;		/* unipolar output */

	dac[1].D_A = 0x000;				/* zero volts */
	dac[1].ADDR = D_A_CHANNEL_1;	/* address of D/A converter */
	dac[1].ZERO = 0x000;			/* integer value for zero volts */
	dac[1].MAX = 10;				/* maximum output is +10 volts */
	dac[1].MIN = 0; 				/* minimum output is zero volts */
	dac[1].POLARITY = UNIPOLAR;		/* unipolar output */

	dac[2].D_A = 0x000;				/* zero volts */
	dac[2].ADDR = D_A_CHANNEL_2;	/* address of D/A converter */
	dac[2].ZERO = 0x000;			/* integer value for zero volts */
	dac[2].MAX = 10;				/* maximum output is +10 volts */
	dac[2].MIN = 0; 				/* minimum output is zero volts */
	dac[2].POLARITY = UNIPOLAR;		/* unipolar output */

	dac[3].D_A = 0x000;				/* zero volts */
	dac[3].ADDR = D_A_CHANNEL_3;	/* address of D/A converter */
	dac[3].ZERO = 0x000;			/* integer value for zero volts */
	dac[3].MAX = 10;				/* maximum output is +10 volts */
	dac[3].MIN = 0;					/* minimum output is zero volts */
	dac[3].POLARITY = UNIPOLAR;		/* unipolar output */

	dac[4].D_A = 0x000;				/* zero volts */
	dac[4].ADDR = D_A_CHANNEL_4;	/* address of D/A converter */
	dac[4].ZERO = 0x000;			/* integer value for zero volts */
	dac[4].MAX = 10;				/* maximum output is +10 volts */
	dac[4].MIN = 0;					/* minimum output is zero volts */
	dac[4].POLARITY = UNIPOLAR;		/* unipolar output */

	dac[5].D_A = 0x000;				/* zero volts */
	dac[5].ADDR = D_A_CHANNEL_5;	/* address of D/A converter */
	dac[5].ZERO = 0x000;			/* integer value for zero volts */
	dac[5].MAX = 10;				/* maximum output is +10 volts */
	dac[5].MIN = 0; 				/* minimum output is zero volts */
	dac[5].POLARITY = UNIPOLAR;		/* unipolar output */

	dac[6].D_A = 0x000;				/* zero volts */
	dac[6].ADDR = D_A_CHANNEL_6;	/* address of D/A converter */
	dac[6].ZERO = 0x000;			/* integer value for zero volts */
	dac[6].MAX = 10;				/* maximum output is +10 volts */
	dac[6].MIN = 0; 				/* minimum output is zero volts */
	dac[6].POLARITY = UNIPOLAR;		/* unipolar output */

	dac[7].D_A = 0x000;				/* zero volts */
	dac[7].ADDR = D_A_CHANNEL_7;	/* address of D/A converter */
	dac[7].ZERO = 0x000;			/* integer value for zero volts */
	dac[7].MAX = 10;				/* maximum output is +10 volts */
	dac[7].MIN = 0; 				/* minimum output is zero volts */
	dac[7].POLARITY = UNIPOLAR;		/* unipolar output */

 #else

 #pragma message("BIRD D/A CONFIGURATION")

	dac[0].D_A = 0x800;				/* zero volts */
	dac[0].ADDR = D_A_CHANNEL_0;	/* address of D/A converter */
	dac[0].ZERO = 0x800;			/* integer value for zero volts */
	dac[0].MAX = 10;				/* maximum output is +10 volts */
	dac[0].MIN = -10;				/* minimum output is zero volts */
	dac[0].POLARITY = BIPOLAR;		/* bipolar output */

	dac[1].D_A = 0x800;				/* zero volts */
	dac[1].ADDR = D_A_CHANNEL_1;	/* address of D/A converter */
	dac[1].ZERO = 0x800;			/* integer value for zero volts */
	dac[1].MAX = 10;				/* maximum output is +10 volts */
	dac[1].MIN = -10;				/* minimum output is zero volts */
	dac[1].POLARITY = BIPOLAR;		/* bipolar output */

	dac[2].D_A = 0x800;				/* zero volts */
	dac[2].ADDR = D_A_CHANNEL_2;	/* address of D/A converter */
	dac[2].ZERO = 0x800;			/* integer value for zero volts */
	dac[2].MAX = 10;				/* maximum output is +10 volts */
	dac[2].MIN = -10;				/* minimum output is zero volts */
	dac[2].POLARITY = BIPOLAR;		/* bipolar output */

	dac[3].D_A = 0x800;				/* zero volts */
	dac[3].ADDR = D_A_CHANNEL_3;	/* address of D/A converter */
	dac[3].ZERO = 0x800;			/* integer value for zero volts */
	dac[3].MAX = 10;				/* maximum output is +10 volts */
	dac[3].MIN = -10;				/* minimum output is zero volts */
	dac[3].POLARITY = BIPOLAR;		/* bipolar output */

	dac[4].D_A = 0x800;				/* zero volts */
	dac[4].ADDR = D_A_CHANNEL_4;	/* address of D/A converter */
	dac[4].ZERO = 0x800;			/* integer value for zero volts */
	dac[4].MAX = 10;				/* maximum output is +10 volts */
	dac[4].MIN = -10;				/* minimum output is zero volts */
	dac[4].POLARITY = BIPOLAR;		/* bipolar output */

	dac[5].D_A = 0x800;				/* zero volts */
	dac[5].ADDR = D_A_CHANNEL_5;	/* address of D/A converter */
	dac[5].ZERO = 0x800;			/* integer value for zero volts */
	dac[5].MAX = 10;				/* maximum output is +10 volts */
	dac[5].MIN = -10;				/* minimum output is zero volts */
	dac[5].POLARITY = BIPOLAR;		/* bipolar output */

	dac[6].D_A = 0x800;				/* zero volts */
	dac[6].ADDR = D_A_CHANNEL_6;	/* address of D/A converter */
	dac[6].ZERO = 0x800;			/* integer value for zero volts */
	dac[6].MAX = 10;				/* maximum output is +10 volts */
	dac[6].MIN = -10;				/* minimum output is zero volts */
	dac[6].POLARITY = BIPOLAR;		/* bipolar output */

	dac[7].D_A = 0x800;				/* zero volts */
	dac[7].ADDR = D_A_CHANNEL_7;	/* address of D/A converter */
	dac[7].ZERO = 0x800;			/* integer value for zero volts */
	dac[7].MAX = 10;				/* maximum output is +10 volts */
	dac[7].MIN = -10;				/* minimum output is zero volts */
	dac[7].POLARITY = BIPOLAR;		/* bipolar output */

 #endif

 }

 #pragma page(1)

 void init_cmd_tm_interface(void)
 {
/****************************************************************************
 *
 *	initialize Command/Telemetry interface
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	initialize Command/Telemetry interface
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	initialize Command/Telemetry interface
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	initialize decoder application
 *
 ***************************************************************************/

 #define        OFF             0
 #define        ON              1

	decoder(OFF);

	int_disable(CMD_IRQ);	/* disable command decoder */

	cmd.frame_count = 0;

	int_enable(CMD_IRQ);	/* enable command decoder */

 }

 void decoder(int on_off)
 {
/****************************************************************************
 *
 *      disable interrupts
 *
 ***************************************************************************/

	int_disable(CMD_IRQ);

/****************************************************************************
 *
 *      Install command decoder function pointer
 *
 ***************************************************************************/

	if(on_off == 0) {
		install_cmd_application(dummy_cmd_application);
		disable_cmd_application();

		install_cmd_irq(dummy_cmd_irq);
		disable_cmd_irq_function();
	}
	else {
		install_cmd_application(data_cmd_application);
		enable_cmd_application();

		install_cmd_irq(data_cmd_irq);
		enable_cmd_irq_function();
	}
/****************************************************************************
 *
 *      unmask interrupt at interrupt controller
 *
 ***************************************************************************/

	int_enable(CMD_IRQ);
 }

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void far data_cmd_application(void)
 {
 }

 #pragma page(1)

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void far data_cmd_irq(void)
 {
 }

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void init_remtron(void)
 {

 }
