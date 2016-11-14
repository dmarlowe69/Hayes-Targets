//  file:   MDS_EX.H
//////////////////////////////////////////////////////////////////////////////
//
//      TLX AUTOPILOT Interface header file
//
//      The varaible defintions in this file are used to pass data
//      from the Cartwright routines to the Hayes Routines
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//  COMMAND DATA TO AUTOPILOT
//

//
//  Preset Altitude - Floating point number for Altitude zero - 500 feet
//
 extern double CMDPRESET;

//
//  Autopilot Gains
//

 #define KP_DEFAULT     0x66
 #define KV_DEFAULT		0x4c
 #define KI_DEFAULT		0x33
 #define KS_DEFAULT		0x50
 #define MR_DEFAULT     0x66

//
//  proportional gain - lower eight bits used 0x00-0xff
//
 extern int PGAIN;
//
//  rate gain - lower eight bits used 0x00-0xff
//
 extern int RGAIN;
//
//  integral gain - lower eight bits used 0x00-0xff
//
 extern int IGAIN;
//
//  scale factor - lower eight bits used 0x00-0xff
//
 extern int SGAIN;
//
//  manual reset - lower eight bits used 0x00-0xff
//
 extern int MGAIN;

//
//  command channel
//

//
//  RCVY/FLY - Recovery/FLY Command
//
 #define RCVY_MODE      0x01    // wings at -1.5 degrees
 #define FLY_MODE       0x00    // Height Keeper mode
 extern int  RCVY_FLY;
//
//  PROG - Autopilot Program Mode
//
 #define PROG_STD       0x00    // Standard Configuration
 #define PROG_GAIN      0x02    // Programmable Configuration
 #define PROG_ALT       0x04    // Swedish Configuration
 #define PROG_MANUAL    0x06    // Manual WIng Control
 extern int PROG_MODE;

//
//  TELEMETRY DATA FROM AUTOPILOT
//

 extern double FALTITUDE;
 extern double FALT_DEV;
 extern double FACCLEROMETER;
 extern double FWING_COMMAND;
 extern int RADAR_LOCK;

 #define RCVY_MODE      0x01    // wings at -1.5 degrees
 #define FLY_MODE       0x00    // Height Keeper mode
 #define ERROR            -1    // Target auto recovery mode
 extern int STATUS_1;

 #define PROG_STD       0x00    // Standard Configuration
 #define PROG_GAIN      0x02    // Programmable Configuration
 #define PROG_ALT       0x04    // Swedish Configuration
 #define PROG_MANUAL    0x06    // Manual WIng Control
 #define ERROR            -1    // Target auto recovery mode
 extern int STATUS_2;
