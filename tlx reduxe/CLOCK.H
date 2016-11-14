/* file: clock.h */

/****************************************************************************
 *
 *      Hardware Specific configuration header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      The CLOCK_RATE define is the frequency of the signal being applied to
 *      the internal counter/timer of the V40 and is dependent upon not only
 *      the master clock frquency but also upon the value programmed into the
 *      TCKS register (address FFF0H) which holds the prescale value.
 *
 *      The prescale value is set in the STARTUP code file HARDINIT.INC, and
 *      is currently programmed to a value of 0x02.
 *
 ***************************************************************************/

 #if !defined(CLOCK)

 #define CLOCK 10

 #pragma message("default clock speed - 10Mz")

 #endif

/****************************************************************************
 *
 *	Set define for V40 series processors
 *
 ***************************************************************************/

 #if defined(V40)

/****************************************************************************
 *
 *      LPM-SBC40 CPU Board Configuration
 *
 ***************************************************************************/

 #if CLOCK == 5

 #define PRESCALE 2L		/* Prescale in TCKS register */

 #define CLOCK_RATE 2500000L	/* 8 MHz / PRESCALE */

 #pragma message("V40 - 5Mz")

 #elif CLOCK == 8

 #define PRESCALE 2L		/* Prescale in TCKS register */

 #define CLOCK_RATE 4000000L	/* 8 MHz / PRESCALE */

 #pragma message("V40 - 8Mz")

 #elif CLOCK == 10

 #define PRESCALE 2L		/* Prescale in TCKS register */

 #define CLOCK_RATE 5000000L    /* 10 MHz / PRESCALE */

 #pragma message("V40 - 10Mz")

 #endif

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #elif defined(V50)

/****************************************************************************
 *
 *      LPM-SBC50 CPU Board Configuration
 *
 ***************************************************************************/

 #define PRESCALE 2L		/* Prescale in TCKS register */

 #define CLOCK_RATE 5000000L    /* 10 MHz / PRESCALE */

 #pragma message("V50 - 10Mz")

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #elif defined(V20)

/****************************************************************************
 *
 *      LPM-SBC8 CPU Board Configuration
 *
 ***************************************************************************/

 #define PRESCALE 1L			/* Prescale in TCKS register */

 #define CLOCK_RATE 8000000L	/* 8 MHz*/

 #pragma message("V20 - 8Mz")

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #elif defined(I486) || defined(PC)

/****************************************************************************
 *
 *		LPM-486SLC CPU Board Configuration
 *
 ***************************************************************************/

 #define PRESCALE 1L			/* Prescale in TCKS register */

 #define CLOCK_RATE 1193181L	/* standard PC rate */

 #pragma message("I486 - STD PC Rate")

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #endif
