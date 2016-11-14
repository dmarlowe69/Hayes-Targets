//  file:   MDS.H
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
 double CMDPRESET = 50.0;

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
 int PGAIN = KP_DEFAULT;
//
//  rate gain - lower eight bits used 0x00-0xff
//
 int RGAIN = KV_DEFAULT;
//
//  integral gain - lower eight bits used 0x00-0xff
//
 int IGAIN = KI_DEFAULT;
//
//  scale factor - lower eight bits used 0x00-0xff
//
 int SGAIN = KS_DEFAULT;
//
//  manual reset - lower eight bits used 0x00-0xff
//
 int MGAIN = MR_DEFAULT;

//
//  command channel
//

//
//  RCVY/FLY - Recovery/FLY Command
//
 #define RCVY_MODE      0x01    // wings at -1.5 degrees
 #define FLY_MODE       0x00    // Height Keeper mode
 int  RCVY_FLY = RCVY_MODE;
//
//  PROG - Autopilot Program Mode
//
 #define PROG_STD       0x00    // Standard Configuration
 #define PROG_GAIN      0x02    // Programmable Configuration
 #define PROG_ALT       0x04    // Swedish Configuration
 #define PROG_MANUAL    0x06    // Manual WIng Control
 int PROG_MODE = PROG_STD;

//
//  TELEMETRY DATA FROM AUTOPILOT
//

 double FALTITUDE;
 double FALT_DEV;
 double FACCLEROMETER;
 double FWING_COMMAND;
 int RADAR_LOCK;

 #define RCVY_MODE      0x01    // wings at -1.5 degrees
 #define FLY_MODE       0x00    // Height Keeper mode
 #define ERROR            -1    // Target auto recovery mode
 int STATUS_1;

 #define PROG_STD       0x00    // Standard Configuration
 #define PROG_GAIN      0x02    // Programmable Configuration
 #define PROG_ALT       0x04    // Swedish Configuration
 #define PROG_MANUAL    0x06    // Manual WIng Control
 #define ERROR            -1    // Target auto recovery mode
 int STATUS_2;
