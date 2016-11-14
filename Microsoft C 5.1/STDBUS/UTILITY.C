/* file: utility.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Utility functions")
 #pragma subtitle("(c)1994 Hayes Targets")

/****************************************************************************
 *
 *      Utility functions
 *
 *      written by denton marlowe
 *      (c)1994 Hayes Targets
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
 #include	<gnd.h>

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      utility data structure
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      utility definitions
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      general bit definitions
 *
 ***************************************************************************/

 #define B0     0x01
 #define B1     0x02
 #define B2     0x04
 #define B3     0x08
 #define B4     0x10
 #define B5     0x20
 #define B6     0x40
 #define B7     0x80

/****************************************************************************
 *
 *      utility function definitions
 *
 ***************************************************************************/

 void init_systems(void);               /* initialize application hardware */
 void systems_init(void);               /* initialize application hardware */
 void systems_on(void);                 /* turn on hardware */
 void systems_off(void);                /* turn off hardware */

 void actuator_flex(void);              /* flex actuators */
 void actuator_init(void);              /* initialize position of actuators */

/****************************************************************************
 *
 *  local global variable (not public)
 *
 ***************************************************************************/

 static int util_debugging_messages = DISABLE;

 void set_util_debugging_messages(int flag);
 int  get_util_debugging_messages(void);

 void utility_menu(void);

/****************************************************************************
 *
 *      define data structures for scaled autopilot outputs
 *
 ***************************************************************************/

 struct CONTROLS {
	signed int actuator_1;
	signed int actuator_2;
	signed int actuator_3;
	};

 struct CONTROLS controls;

/****************************************************************************
 *
 *      application function definitions
 *
 ***************************************************************************/

 void actuator_position(struct CONTROLS *controls);
				/* send commands to actuators */

 #pragma page(1)

/***************************************************************************
 *
 *  Initialize application specific hardware
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 **************************************************************************/
 void init_systems(void)
 {
	systems_init();
 }

 #pragma page(1)

/***************************************************************************
 *
 *  Initialize application specific hardware
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 **************************************************************************/
 void systems_init(void)
 {

    util_debugging_messages = ENABLE;

    if(util_debugging_messages == ENABLE)
        com_puts("\r\ninit_systems\r\n");

 }

 #pragma page(1)

/***************************************************************************
 *
 *  Turn On specific hardware
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 **************************************************************************/
 void systems_on(void)
 {
    if(util_debugging_messages == ENABLE)
        com_puts("\r\nsystems_on\r\n");

 }

 #pragma page(1)

/***************************************************************************
 *
 *  Turn Off specific hardware
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 **************************************************************************/
 void systems_off(void)
 {
    if(util_debugging_messages == ENABLE)
        com_puts("\r\nsystems_off\r\n");

 }

 #pragma page(1)

/***************************************************************************
 *
 *  actuator flex test
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void actuator_flex(void)
 {
	int i;

    if(util_debugging_messages == ENABLE)
        com_puts("\r\nactuator_flex\r\n");

    for( i=0; i<4096; i+=16)
		{
		controls.actuator_1 = i;
		controls.actuator_2 = i;
		controls.actuator_3 = i;
		actuator_position(&controls);
		delay(4096);
        if(util_debugging_messages == ENABLE)
            com_puts(".");
        }

    if(util_debugging_messages == ENABLE)
	    com_puts("\r\n");

    for( i=4095; i>=0; i-=16)
		{
		controls.actuator_1 = i;
		controls.actuator_2 = i;
		controls.actuator_3 = i;
		actuator_position(&controls);
		delay(4096);
        if(util_debugging_messages == ENABLE)
            com_puts("!");
        }

	delay(32000);
	delay(32000);
	delay(32000);
	delay(32000);

	controls.actuator_1 = 0xfff;
	controls.actuator_2 = 0xfff;
	controls.actuator_3 = 0xfff;
	actuator_position(&controls);

	delay(32000);
	delay(32000);
	delay(32000);
	delay(32000);

	controls.actuator_1 = 0;
	controls.actuator_2 = 0;
	controls.actuator_3 = 0;
	actuator_position(&controls);

	delay(32000);
	delay(32000);
	delay(32000);
	delay(32000);

    actuator_init();

 }

 #pragma page(1)

/***************************************************************************
 *
 *  actuator initialization
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void actuator_init(void)
 {
    if(util_debugging_messages == ENABLE)
    	com_puts("\r\nactuator_init\r\n");

    controls.actuator_1 = 0x800;
	controls.actuator_2 = 0x800;
	controls.actuator_3 = 0x800;
	actuator_position(&controls);
 }

 #pragma page(1)

/***************************************************************************
 *
 *      Position actuator through D/A board
 *
 *      written by denton marlowe
 *      (c)1994 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void actuator_position(struct CONTROLS *controls)
 {
 #if defined(UNDER_DOS)

	return;
 #else

	D_A_channel_out(0,(*controls).actuator_1);      /* rudder */
	D_A_channel_out(1,(*controls).actuator_2);      /* elevator */
	D_A_channel_out(2,(*controls).actuator_3);      /* aileron */

 #endif
 }

 void set_util_debugging_messages(int flag)
 {
    util_debugging_messages = flag;
 }

 int get_util_debugging_messages(void)
 {
    return(util_debugging_messages);
 }

 #pragma page(1)

/****************************************************************************
 *
 *  utility functions
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 ***************************************************************************/
 void utility_menu(void)
 {
 #ifndef _EPROM
 #pragma message("run time debugging included")

	int c;

	while(TRUE)
	{
        com_puts("\r\n\r\nUTILITY FUNCUTIONS\r\n");

        com_puts("\r\nD/E/T. DISABLE/ENABLE Debugging Messages\r\n");

        com_puts("\r\n9. exit\r\n");

        com_puts("\r\n    Make Selection ");

		c = com_getc();         /* get a character from the serial port */

		switch(toupper(c))
		{

            case 'D':
                    util_debugging_messages = DISABLE;
                    break;

            case 'E':
                    util_debugging_messages = ENABLE;
                    break;

            case 'T':
                    if(util_debugging_messages == DISABLE)
                        util_debugging_messages = ENABLE;
                    else
                        util_debugging_messages = DISABLE;

                    break;

            case '9':
                	return;

            default:
					break;
		}
	}

 #endif
 }
