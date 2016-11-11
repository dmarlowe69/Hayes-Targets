/* file: rdeb.h */

/***************************************************************************
 *
 *      RDEB interface functions header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/

/****************************************************************************
 *
 *      startup function prototypes
 *
 ***************************************************************************/

 void far _astart(void);                        /* C startup code function */

/****************************************************************************
 *
 *      C_THRU_ROM Boot fucntion prototype
 *
 ***************************************************************************/

 void C_THRU_ROM(void);                 /* Resident remote debugger */

/****************************************************************************
 *
 *      C_THRU_ROM resident kernel function prototype
 *
 ***************************************************************************/

 void r_main(void);             /* main function of C_THRU_ROM kernel */

/****************************************************************************
 *
 *      RDEB interface definitions
 *
 ***************************************************************************/

 #define KERNEL_PUTS 0xF0       /* The interrupt # handled by the Kernel */

/****************************************************************************
 *
 *      RDEB interface function definitions
 *
 ***************************************************************************/

 int rdeb_putstr(char *str);    /* output string to RDEB console*/
 void rdeb_puts(char *string);  /* output string with newline */
 void rdeb_putch(char c);       /* output character to RDEB console */

/****************************************************************************
 *
 *		CALIBRATE Boot fucntion prototype
 *
 ***************************************************************************/

 void CALIBRATE(void);				   /* Resident remote calibration */

/****************************************************************************
 *
 *      end of RDEB interface functions header file
 *
 ***************************************************************************/
