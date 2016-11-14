/* file: cmd_tm.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("cmd_tm function")
 #pragma subtitle("(c)1990 Hayes Targets")

/****************************************************************************
 *
 *      cmd_tm function
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      This Software was Designed using the following development tools:
 *
 *      MS-DOS Verison 3.3
 *      Microsoft C Version 5.1
 *      MASM Version 5.1
 *      Datalight C_Thru_ROM Version 5.1
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

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include   <hayes.h>
 #include   <gnd.h>

 #include   "platform.h"
 #include   "utility.h"

 #include   "pilot.h"
 #include	"ground.h"

 #ifdef _UART_PORT
 #ifdef V40
 #include   "uart_cmd.h"
 #endif
 #endif
/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Test for 240KHz encoder flag
 *
 ***************************************************************************/

 #if defined(FAST_ENCODER)

 #pragma message("USING 240KHz ENCODER")

 #else

 #pragma message("USING STANDARD ENCODER")

 #endif

/****************************************************************************
 *
 *      Test for LED Debug
 *
 ***************************************************************************/

 #ifdef _EPROM
 #undef LED_DEBUG
 #pragma message("NO STATUS LED")
 #endif

 #ifdef LED_DEBUG

 #pragma message("USING STATUS Board for LED")

 #define DEBUG_STATUS_BANK              0
 #define DEBUG_ERROR_COUNT_BANK         1

 #define DEBUG_TM_DATA_BANK             2
 #define DEBUG_CMD_DATA_BANK            3

 #define DEBUG_CMD_TM_LED_ERROR         0
 #define DEBUG_CMD_TM_LED_STATUS        1

 #endif

/***************************************************************************
 *
 *      general defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

 #undef 	OFF
 #undef     ON

 #define    OFF             0
 #define    ON              1

/***************************************************************************
 *
 *      defintions
 *
 ***************************************************************************/

 #ifdef FAST_ENCODER

 #define CMD_WATCH_COUNT                128U
 #define TM_WATCH_COUNT                 128U

 #else

 #define CMD_WATCH_COUNT                32U
 #define TM_WATCH_COUNT                 16U

 #endif

/***************************************************************************
 *
 *  cmd_tm general definitions
 *
 **************************************************************************/

 #define MANUAL_ON  1
 #define MANUAL_OFF 0

 #define THUMB_WHEEL	 0
 #define NOT_THUMB_WHEEL 1

/***************************************************************************
 *
 *	data structures
 *
 ***************************************************************************/

 unsigned int cmd_status;
 unsigned int tm_status;

/***************************************************************************
 *
 *	define decoded command variables
 *
 **************************************************************************/

 struct CMD_DATA {
    signed int airspeed;
    signed int proportional_gain;
    signed int rate_gain;
    signed int integral_gain;
    signed int delta_scale;
    signed int delta_ec_manual;

    signed int rcvy_fly;
    signed int prog_mode;
    signed int fire_mode;

    unsigned char command;          /* command channel */

    unsigned char channel_mode;     /* rte1/rte2 channel mode */

	unsigned int frame_count;		/* command data frame count */

    unsigned char tm_mode;          /* telemetry mode */
    unsigned char cmd_echo;         /* command word echo */
	unsigned char status_flag;      /* status word */

	unsigned int uart_cmd;			/* uart command mode flag */

	unsigned long int watch_dog;	/* watch dog counter for cmd/tm */
	unsigned long int watch_count;	/* watch dog counter for cmd/tm */
	};

 struct CMD_DATA cmd_data;          /* assign storage */

/****************************************************************************
 *
 *  cmd/tm utilities
 *
 ***************************************************************************/

 int cmd_watch_dog(void);
 int tm_watch_dog(void);

 void decoder(int on_off);
 void encoder(int on_off);

/***************************************************************************
 *
 *	application hooks
 *
 ***************************************************************************/

 void far data_cmd_application(void);
 void far data_tm_application(void);
 void far data_cmd_irq(void);
 void far data_tm_irq(void);

/****************************************************************************
 *
 *	application function definitions
 *
 ***************************************************************************/

 void init_cmd_mode(void);            /* initialize command mode */
 void cmd_processor(void);            /* command processor */

 void far decode_cmd_data(void);      /* decode command data */

 void load_tm_stack(unsigned char tm_mode, unsigned char cmd_echo, unsigned char status_flag);
 void load_status_flag(void);

/****************************************************************************
 *
 *      end of command/telemetry definitions
 *
 ***************************************************************************/

 void prompt_cmd_tm_port(void);

 void far decoder_application(void);

/****************************************************************************
 *
 *  local global variable (not public)
 *
 ***************************************************************************/

 static int cmd_debugging_messages = DISABLE;

 void set_cmd_debugging_messages(int flag);
 int  get_cmd_debugging_messages(void);

 #pragma page(1)

/****************************************************************************
 *
 *  initialize Command/Telemetry interface

 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 ***************************************************************************/
 void init_cmd_tm(void)
 {
 int port=1;
 int i;
/****************************************************************************
 *
 *	initialize Command/Telemetry interface
 *
 ***************************************************************************/

    //prompt_cmd_tm_port();

    // TLX Hardware configuration
    open_cmdtm_port(ID_LPM_SBX_PIO_J1_J2);
    com_puts("ID_LPM_SBX_PIO_J1_J2\r\n");

/****************************************************************************
 *
 *      initialize application
 *
 ***************************************************************************/

	int_disable(CMD_IRQ);   /* disable command decoder */
	int_disable(TM_IRQ);    /* disable command encoder */

    cmd_status = 0;
	cmd.frame_count = 0;

/****************************************************************************
 *
 *      enable interrupts
 *
 ***************************************************************************/

	int_enable(CMD_IRQ);    /* enable command decoder */
    int_enable(TM_IRQ);     /* enable command encoder */

/****************************************************************************
 *
 *  enable interrupt hooks
 *
 ***************************************************************************/

    decoder(ON);
    encoder(ON);

/****************************************************************************
 *
 *  install decoder debugger
 *
 ***************************************************************************/

    install_decoder_application(decoder_application);

/****************************************************************************
 *
 *  enable debugging messages
 *
 ***************************************************************************/

    cmd_debugging_messages = DISABLE;
 }

 #pragma page(1)

/****************************************************************************
 *
 *      CMD Watch Dog - HUD Telemetry Link for RTD1
 *
 *      written by denton marlowe
 *      (c)1994 Hayes Targets
 *
 ***************************************************************************/
 int cmd_watch_dog(void)
 {
    if (cmd.frame_count < 8) {              /* check command counter */
        cmd_status += 1;                    /* increment command check counter */

		if (cmd_status > CMD_WATCH_COUNT) { /* check cmd/tm watch dog */
            //cmd_status = 0;               /* reset to zero */
			cmd_status = CMD_WATCH_COUNT;   /* set to max */
            return(1);                      /* return ERR */
			}
        return(0);                          /* return OK */
        }

    cmd.frame_count = 0;                    /* reset command counter to zero */
    cmd_status = 0;                         /* reset to zero */
    return(0);                              /* return OK */
 }

 #pragma page(1)

/****************************************************************************
 *
 *      TM Watch Dog - HUD Command Link for RTE1
 *
 *      written by denton marlowe
 *      (c)1994 Hayes Targets
 *
 ***************************************************************************/
 int tm_watch_dog(void)
 {
	if(tmApp.frame_count_flag == FRAME_COUNT_OFF)
		return(0);

 #if defined(TM_FRAME_COUNT)
    if (tm.data[TM_FRAME_COUNT] < 8) {      /* check command counter */
 #else
    if (tm.frame_count < 8) {               /* check command counter */
 #endif
        tm_status += 1;                     /* increment command check counter */

        if (tm_status > TM_WATCH_COUNT) {   /* check cmd/tm watch dog */
            tm_status = 0;                  /* reset to zero */
            return(1);                      /* return ERR */
			}
        return(0);                          /* return OK */
		}

 #if defined(TM_FRAME_COUNT)
    tm.data[TM_FRAME_COUNT] = 0;            /* reset command counter to zero */
 #else
    tm.frame_count = 0;                     /* reset command counter to zero */
 #endif
    tm_status = 0;                          /* reset to zero */
    return(0);                              /* return OK */
 }

 #pragma page(1)

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

	if(on_off == OFF) {
		install_cmd_application(dummy_cmd_application);
		disable_cmd_application();

		install_cmd_irq(dummy_cmd_irq);
		disable_cmd_irq_function();
 #ifndef _EPROM
        reset_LED(0,6);
 #endif
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

 void encoder(int on_off)
 {
/****************************************************************************
 *
 *      disable interrupts
 *
 ***************************************************************************/

	int_disable(TM_IRQ);

/****************************************************************************
 *
 *      Install command decoder function pointer
 *
 ***************************************************************************/

	if(on_off == OFF) {
		install_tm_application(dummy_tm_application);
		disable_tm_application();

		install_tm_irq(dummy_tm_irq);
		disable_tm_irq_function();
 #ifndef _EPROM
        reset_LED(0,7);
 #endif
	}
	else {
		install_tm_application(data_tm_application);
		enable_tm_application();

		install_tm_irq(data_tm_irq);
		enable_tm_irq_function();
	}

/****************************************************************************
 *
 *      unmask interrupt at interrupt controller
 *
 ***************************************************************************/

	int_enable(TM_IRQ);
 }

 #pragma page(1)

/****************************************************************************
 *
 *  command data decoder
 *
 ***************************************************************************/
 void far data_cmd_application(void)
 {
	decode_cmd_data();

 #ifndef _EPROM
    write_LED(DEBUG_CMD_DATA_BANK,cmd.data[9]);
 #endif
 }

/****************************************************************************
 *
 *  telemetry data loader
 *
 ***************************************************************************/
 void far data_tm_application(void)
 {
 static int i;

	//for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
	//	tm.data[i] = (tm.data[i] + 1)&0xff;
	//}

 #ifndef _EPROM
    write_LED(DEBUG_TM_DATA_BANK,tm.data[0]);
 #endif

	load_tm_stack(cmd_data.tm_mode,cmd_data.cmd_echo,cmd_data.status_flag);
 }

 #pragma page(1)

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void far data_cmd_irq(void)
 {
 #ifndef _EPROM
    toggle_LED(0,6);
 #endif
 }

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void far data_tm_irq(void)
 {
 #ifndef _EPROM
    toggle_LED(0,7);
 #endif
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Command mode initialization
 *
 *      DELCARATION:    void init_cmd_mode(void);
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
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
 *       Referenced:    cmd             command data structure
 *                      cmd_data        command data structure
 *
 *                      void cmd_tm_reset(void);
 *                      void comm_port(void);
 *
 *      HEADERS:        "cmd_tm.h"
 *                      "ground.h"
 *                      <hayes.h>
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         cmd_tm.c
 *
 *      DESCRIPTION:
 *
 *      This routine synchronizes the remote control console to the command
 *      decoder. Utill sync is established the deafault tm mode (echo command
 *      data) is in effect. The comm port is also tested for ground test
 *      terminal brake. The configuration is application specific.
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_cmd_mode(void)
 {
/****************************************************************************
 *
 *      initialize command link
 *
 ***************************************************************************/

	cmd_data.tm_mode = 0;			/* initialize telemetry mode */
    cmd_data.cmd_echo = 0x00;       /* initialize command echo word */
	cmd_data.status_flag = 0x00;	/* initialize status word */

	cmd_data.channel_mode = 0x09;	/* rte1 9 channel mode */

    cmd_data.uart_cmd = DISABLE;    /* disable uart command mode */

//
    cmd_debugging_messages = ENABLE;

/****************************************************************************
 *
 *  wait for command word first sync byte
 *
 *  note the // comments are used for serial cmd/tm protocal
 *
 ***************************************************************************/

    while( (cmd.data[9]&0x01) != 0x01 )
		{
		if( cmd.frame_count > 8 )
			{
			cmd.frame_count = 0;            /* reset command counter */
            cmd_data.cmd_echo = cmd_data.cmd_echo ^ 0x80;
							/* blink command bits */
            cmd_data.status_flag = cmd_data.status_flag ^ 0x80;
							/* blink status bits */
			}

		comm_port();							/* test comm ports */
    	if(cmd_debugging_messages == ENABLE)
        	com_puts("?");

 #ifdef _UART_PORT
 #ifdef V40

        if(cmd_data.uart_cmd == DISABLE) {
            cmd_data.uart_cmd = uart_cmd_port();
            }

        if(cmd_data.uart_cmd == USE) {
            uart_cmd_initialize();
            cmd_data.uart_cmd = ENABLE;
            cmd.data[9] = 0x01;
            uart_puts("\r\nUart CMD Port ENABLE 0x01\r\n");
            break;
            }
 #endif
 #endif

        } /* while */

 #ifdef _UART_PORT
 #ifdef V40

    if(cmd_data.uart_cmd == ENABLE) {
        uart_puts("\r\nUart out of first loop\r\n");
        }
 #endif
 #endif

/****************************************************************************
 *
 *	determine command encoder channel configuration
 *
 *  note the // comments are used auto incomming data channel count
 *
 ***************************************************************************/

 #ifdef _UART_PORT
 #ifdef V40

    if(cmd_data.uart_cmd == ENABLE) {
        uart_puts("\r\nUart CMD Port Selected\r\n");
        }
    else {
        cmd_data.channel_mode = identify_command_encoder();
        }

 #endif
 #endif

/****************************************************************************
 *
 *	determine command encoder channel configuration
 *
 ***************************************************************************/
 //
 //	cmd_data.channel_mode = identify_command_encoder();
 //
/****************************************************************************
 *
 *      setup default command mode
 *
 ***************************************************************************/

	cmd_data.tm_mode = 1;                   /* autopilot telemetry mode */
    cmd_data.cmd_echo = 0x00;               /* initialize command echo word */
	cmd_data.status_flag = 0x00;            /* initialize status word */
    cmd_data.watch_count = 320000U;
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      command processor
 *
 *      DELCARATION:    void cmd_processor(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
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
 *       Referenced:    cmd             command data structure
 *                      cmd_data        command data structure
 *
 *                      void cmd_tm_reset(void);
 *                      void recovery_status(void);
 *                      void cmd_mode_0(void);
 *                      void cmd_mode_1(void);
 *                      void cmd_mode_2(void);
 *                      void cmd_mode_3(void);
 *                      void cmd_mode_4(void);
 *
 *      HEADERS:        "cmd_tm.h"
 *                      "utility.h"
 *                      <hayes.h>
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         cmd_tm.c
 *
 *      DESCRIPTION:
 *
 *      This routine excutes the command functions called through the command
 *      link data decoder. The command functions are only accessed every
 *      eight frames. The routine also watch dogs the command decoder.
 *      The configuration is application specific.
 *
 *  note the // comments are used auto incomming data channel count
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ************************************************************************@H*/
 void cmd_processor(void)
 {
 int uart_status;

	if (cmd.frame_count < 8)			/* check command counter */
		{
		cmd_data.watch_dog += 1;        /* increment command check counter */

        if (cmd_data.watch_dog > cmd_data.watch_count)  /* check cmd/tm watch dog for 10 seconds*/
			{
			cmd_data.watch_dog = 0; /* reset to zero */
            cmd_tm_reset();         /* reset command and telemetry */
            set_pilot_safe_mode(ENABLE);
			com_puts("\r\nAUTOPILOT SAFE MODE ENABLED\r\n");
            }
			comm_port();						/* test comm port */

 #ifdef _UART_PORT
 #ifdef V40
        	if(cmd_data.uart_cmd == DISABLE) {
            	uart_port();                    /* test comm port */
        	}
 #endif
 #endif
		return;                         /* return */
		}

/***************************************************************************
 *
 *	adjust watch dog counter for autopilot mode
 *
 ***************************************************************************/

	if(get_autopilot_mode() == MANUAL)
		cmd_data.watch_count = 320000U;
	else
		cmd_data.watch_count = 106667U;

/***************************************************************************
 *
 *	test serial comm ports for received commands
 *
 ***************************************************************************/

	comm_port();						/* test comm port */

 #ifdef _UART_PORT
 #ifdef V40
    if(cmd_data.uart_cmd == DISABLE) {
        uart_port();                    /* test comm port */
        }
 #endif
 #endif

/***************************************************************************
 *
 *	test serial comm ports for received commands
 *
 ***************************************************************************/

 #ifdef _UART_PORT
 #ifdef V40
    if(cmd_data.uart_cmd == ENABLE) {
        cmd_data.watch_dog = 0;         /* reset to zero */
        cmd.frame_count = 8;            /* set command counter */

        uart_status = uart_cmd();

        if( uart_status == NODATA ) {
            return;
            }

        if( uart_status == DISUSE) {
            cmd_data.uart_cmd = DISABLE;
            return;
          }
        }
    else {
 #endif
 #endif
        cmd.frame_count = 0;            /* reset command counter to zero */
		cmd_data.watch_dog = 0; 		/* reset command check counter to zero */
 #ifdef _UART_PORT
 #ifdef V40
        }
 #endif
 #endif

/***************************************************************************
 *
 *      reset command data word counter
 *
 ***************************************************************************/

    load_status_flag();

	cmd_data.cmd_echo = cmd_data.command;

/***************************************************************************
 *
 *      ok to process commands
 *
 ***************************************************************************/

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *  Command data decoder
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 ************************************************************************@H*/
 void far decode_cmd_data(void)
 {
    //
    //  Thumb wheel is ch1, ch2 and ch3
    //
    cmd_data.airspeed = ((cmd.data[1]^0xff)/27)*1 \
                      + ((cmd.data[2]^0xff)/27)*10 \
                      + ((cmd.data[3]^0xff)/27)*100;

    //
    //  ch4 - proportional gain
    //
    cmd_data.proportional_gain = cmd.data[6];

    //
    //  ch5 - rate gain
    //
    cmd_data.rate_gain = cmd.data[5];

    //
    //  ch6 - integral gain
    //
    cmd_data.integral_gain = cmd.data[6];

    //
    //  ch7 - scale factor
    //
    cmd_data.delta_scale = ( (cmd.data[7] & 0xf0) >> 4 ) + 1;

    //
    //  ch8 - manual reset
    //
    cmd_data.delta_ec_manual = cmd.data[8] * cmd_data.delta_scale;

    //
    //  ch9 - command channel
    //
    cmd_data.command = cmd.data[9];

    //
    //  RCVY/FLY
    //
    cmd_data.rcvy_fly = cmd.data[9] & RCVY_FLY_BITS;

    //
    //  PROG
    //
    cmd_data.prog_mode = cmd.data[9] & PROG_BITS;

    //
    //  FIRE
    //
    cmd_data.fire_mode = cmd.data[9] & FIRE_BITS;

    //
    //  frame counter
    //
    cmd_data.frame_count++;
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Telemetry data Loader
 *
 *      written by denton marlowe
 *      (c)1994 Hayes Targets
 *
 ************************************************************************@H*/
 void load_tm_stack(unsigned char tm_mode, unsigned char cmd_echo, unsigned char status_flag)
 {
 static int i;

	switch(tm_mode)
	{
/***************************************************************************
 *
 *      telemetry mode 0: echo command link data
 *
 ***************************************************************************/

	case 0:

    tm.data[0] = cmd.data[1];       /* address  1 out of telemetry decoder */
	tm.data[1] = cmd.data[2];       /* address  2 out of telemetry decoder */
	tm.data[2] = cmd.data[3];       /* address  3 out of telemetry decoder */
	tm.data[3] = cmd.data[4];       /* address  4 out of telemetry decoder */
	tm.data[4] = cmd.data[5];       /* address  5 out of telemetry decoder */
	tm.data[5] = cmd.data[6];       /* address  6 out of telemetry decoder */
	tm.data[6] = cmd.data[7];       /* address  7 out of telemetry decoder */
	tm.data[7] = cmd.data[8];       /* address  8 out of telemetry decoder */

	tm.data[8] = cmd_echo;          /* address  9 out of telemetry decoder */

    break;

/***************************************************************************
 *
 *	telemetry mode 1:
 *
 ***************************************************************************/

	case 1:

    tm.data[0] = cmd.data[1];       /* address  1 out of telemetry decoder */
	tm.data[1] = cmd.data[2];       /* address  2 out of telemetry decoder */
	tm.data[2] = cmd.data[3];       /* address  3 out of telemetry decoder */
	tm.data[3] = cmd.data[4];       /* address  4 out of telemetry decoder */
	tm.data[4] = cmd.data[5];       /* address  5 out of telemetry decoder */
	tm.data[5] = cmd.data[6];       /* address  6 out of telemetry decoder */
	tm.data[6] = cmd.data[7];       /* address  7 out of telemetry decoder */
	tm.data[7] = cmd.data[8];       /* address  8 out of telemetry decoder */

    tm.data[8] = status_flag;       /* address  9 out of telemetry decoder */

    break;

/***************************************************************************
 *
 *	telemetry mode 2:
 *
 ***************************************************************************/

	case 2:

    break;

/***************************************************************************
 *
 *	telemetry mode 3:
 *
 ***************************************************************************/

	case 3:

        break;

/***************************************************************************
 *
 *	telemetry mode 4:
 *
 ***************************************************************************/

	case 4:

		for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
			tm.data[i] = i;
			}

		break;

/***************************************************************************
 *
 *	telemetry mode 5:
 *
 ***************************************************************************/

	case 5:

		for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
			tm.data[i] = (tm.data[i] + 1)&0xff;
			}

		break;

/***************************************************************************
 *
 *	end of switch block
 *
 ***************************************************************************/

	}		/* switch */

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Telemetry data Loader
 *
 *      written by denton marlowe
 *      (c)1994 Hayes Targets
 *
 ************************************************************************@H*/
 void load_status_flag(void)
 {
	cmd_data.status_flag = cmd_data.status_flag ^ 0x0f;
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *  Prompt for cmd_tm Port
 *
 *  written by denton marlowe
 *  (c)1990 Hayes Targets
 *
 ************************************************************************@H*/
 void prompt_cmd_tm_port(void)
 {
 #ifndef _EPROM
 #pragma message("run time debugging included")

 int port=1;
/****************************************************************************
 *
 *	initialize Command/Telemetry interface
 *
 ***************************************************************************/

	com_puts("Interface Port Definition\r\n");

 #if defined(I486)

	com_puts("press 's' for sbx 'l' for lpm-sbx or 'p,q,r' for pcmj2,1,2\r\n");
	com_puts("(0) LPM-SBX #1 J1\r\n");
	com_puts("(1) LPM-SBX #1 J2\r\n");
	com_puts("(2) LPM-SBX #2 J1\r\n");
	com_puts("(3) LPM-SBX #2 J2\r\n");

 #else

	com_puts("press 's' for single-sbx, 'd' for dual-sbx, 'l' for lpm-sbx\r\n");

 #endif

	port = com_getc();

	switch(toupper(port)) {

		case '0':
			open_cmdtm_port(ID_LPM_SBX_PIO_J1_J1);
			com_puts("ID_LPM_SBX_PIO_J1_J1\r\n");
			break;

		case '1':
			open_cmdtm_port(ID_LPM_SBX_PIO_J1_J2);
			com_puts("ID_LPM_SBX_PIO_J1_J2\r\n");
			break;

		case '2':
			open_cmdtm_port(ID_LPM_SBX_PIO_J2_J1);
			com_puts("ID_LPM_SBX_PIO_J2_J1\r\n");
			break;

		case '3':
			open_cmdtm_port(ID_LPM_SBX_PIO_J2_J2);
			com_puts("ID_LPM_SBX_PIO_J2_J2\r\n");
			break;

 #ifndef I486
		case 'D':
			open_cmdtm_port(ID_SBX_PIO);
			com_puts("ID_SBX_PIO\r\n");
			break;
 #endif

		case 'S':
			//open_cmdtm_port(ID_ONBOARD_PIO);
			//com_puts("ID_ONBOARD_PIO\r\n");

			//open_cmdtm_port(ID_LPM_SBX_PIO_J2_J2);
			//com_puts("ID_LPM_SBX_PIO_J2_J2\r\n");
			open_cmdtm_port(ID_LPM_SBX_PIO_J2_J1);
			com_puts("ID_LPM_SBX_PIO_J2_J1\r\n");

			//open_cmdtm_port(ID_LPM_SBX_PIO_J1_J2);
			//com_puts("ID_LPM_SBX_PIO_J1_J2\r\n");
			break;

		case 'L':
			open_cmdtm_port(ID_LPM_SBX_PIO);
			com_puts("ID_LPM_SBX_PIO\r\n");
			//open_cmdtm_port(ID_LPM_SBX_PIO_J2_J2);
			//com_puts("ID_LPM_SBX_PIO_J2_J2\r\n");
			break;

 #if defined(I486)
		case 'P':
			open_cmdtm_port(ID_PCM_PIO);
			com_puts("ID_PCM_PIO\r\n");
			break;

		case 'Q':
			open_cmdtm_port(ID_PCM_PIO_J1);
			com_puts("ID_PCM_PIO_J1\r\n");
			break;

		case 'R':
			open_cmdtm_port(ID_PCM_PIO_J2);
			com_puts("ID_PCM_PIO_J2\r\n");
			break;
 #endif

		default:
			open_cmdtm_port(ID_SBX_PIO);
			com_puts("ID_SBX_PIO\r\n");
			break;
		}

 #endif
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *  Prompt for mit Port
 *
 *  written by denton marlowe
 *  (c)1990 Hayes Targets
 *
 ************************************************************************@H*/
 void far decoder_application(void)
 {
 #ifndef _EPROM
 #pragma message("run time debugging included")

	com_puts("\r\ndecoder debugger\r\n");

    display_CMD();

    printf("\r\n");

    printf("cmd_data.airspeed %d\r\n",cmd_data.airspeed);

    printf("\r\n");

    printf("cmd_data.proportional_gain %d\r\n",cmd_data.proportional_gain);

    printf("\r\n");

    printf("cmd_data.rate_gain %d\r\n",cmd_data.rate_gain);

    printf("\r\n");

    printf("cmd_data.integral_gain %d\r\n",cmd_data.integral_gain);

    printf("\r\n");

    printf("cmd_data.delta_scale %d\r\n",cmd_data.delta_scale);

    printf("\r\n");

    printf("cmd_data.delta_ec_manual %d\r\n",cmd_data.delta_ec_manual);

    printf("\r\n");

    printf("cmd_data.command %d\r\n",cmd_data.command);

	pause();

 #endif
 }

 #pragma page(1)

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
 void set_cmd_debugging_messages(int flag)
 {
    cmd_debugging_messages = flag;
 }

 int get_cmd_debugging_messages(void)
 {
    return(cmd_debugging_messages);
 }
