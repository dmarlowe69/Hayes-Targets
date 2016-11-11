/* file: platform.h */

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

 void ramtest(void);

/****************************************************************************
 *
 *	cmd/tm functions
 *
 ***************************************************************************/

 void init_cmd_tm_interface(void);
 void decoder(int on_off);
 void far data_cmd_application(void);
 void far data_cmd_irq(void);
