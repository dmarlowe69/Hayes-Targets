/* file: cmd_tm.h */

/****************************************************************************
 *
 *      Command/Telemetry header file
 *
 *      written by denton marlowe
 *      (c)1989,1990 Hayes Targets
 *
 ***************************************************************************/

/***************************************************************************
 *
 *      define decoded command variables
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

 extern struct CMD_DATA cmd_data;

/****************************************************************************
 *
 *      command/telemetry function prototypes
 *
 ***************************************************************************/

 void init_cmd_tm(void);
 void init_cmd_mode(void);            /* initialize command mode */
 void cmd_processor(void);            /* command processor */

 void far decode_cmd_data(void);      /* decode command data */

 void load_tm_stack(unsigned char tm_mode, unsigned char cmd_echo, unsigned char status_flag);
 void load_status_flag(void);

 void prompt_cmd_tm_port(void);

 void far decoder_application(void);
