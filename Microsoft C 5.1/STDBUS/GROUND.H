// file: ground.h
//

/****************************************************************************
 *
 *      Ground function prototypes
 *
 ***************************************************************************/

 void reset_boot(void);					/* cold boot */
 void reset_kernel(void);

 void comm_port(void);					/* test comm ports for access */
 void ground_menu(void);

 void _power_on_reset(void);
 void _kernel_reset(void);
