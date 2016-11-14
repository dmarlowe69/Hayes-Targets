/* file: utility.h */

 #pragma message("UTILITY.H")

/****************************************************************************
 *
 *      Utility header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

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

 extern struct CONTROLS controls;

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
 void actuator_init(void);              /* initialize position of actuatros */

 void actuator_position(struct CONTROLS *controls);
				/* send commands to actuators */

 void set_util_debugging_messages(int flag);
 int  get_util_debugging_messages(void);
 void utility_menu(void);
