/* file: rtclock.c */

/***************************************************************************
 *
 *	Standard headers
 *
 ***************************************************************************/

/***************************************************************************
 *
 *	Microsoft  headers
 *
 ***************************************************************************/

 #include	<dos.h>
 #include	<conio.h>

/***************************************************************************
 *
 *	local headers
 *
 ***************************************************************************/

 #include	"rtclock.h"

/***************************************************************************
 *
 *	definitions
 *
 ***************************************************************************/

 #define CMOSFLAG 	0x70
 #define CMOSDATA 	0x71
 #define STATUSA  	0x0a
 #define RTC_VEC  	0x70
 #define IMR2		0xa1
 #define CMD1		0x20
 #define CMD2		0xa0
 #define EOI		0x20
 #define RTC_MASK 	0xfe
 #define STATUSB  	0x0b
 #define STATUSC  	0x0c
 #define RTC_FLAG 	0x40

/****************************************************************************
 *
 *	globals
 *
 ***************************************************************************/

 int RtcCounter;
 unsigned char OldMaskA,OldMaskB;

 static int clk_hertz,RtcSeconds;

/****************************************************************************
 *
 *	function pointers
 *
 ***************************************************************************/

 void (interrupt far *OldRtcVec)();
 void (far *NewRtcVec)();

 void EnableRtcInts(int ClockSpeed);
 void DisableRtcInts(void);

 static void rtc_timer();

 void rtc_open(void);
 void rtc_close(void);
 unsigned long int rtctime(void);
 double rtctimed(void);

 double rtc_tcks_per_second(void);
 double rtc_seconds_per_tick(void);

/****************************************************************************
 *
 *	Prototypes
 *
 ***************************************************************************/

 void interrupt far RtcHook(void);

/****************************************************************************
 *
 *	interrupt service routine
 *
 ***************************************************************************/
 void interrupt far RtcHook(void)
 {
	_disable(); 	// redundant

	outp(CMOSFLAG,STATUSC); 	// is this interrupt for us?
	if(inp(CMOSDATA) & 0x40 ) {
		NewRtcVec();			// call new rtc isr
		outp(CMD1,EOI); 		// primary 8259
		outp(CMD2,EOI); 		// slave 8259
		}
	else {
		OldRtcVec();			// call old rtc isr
		}

	_enable();					// enable irq's
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void EnableRtcInts(int ClockSpeed)
 {
 // define clock rate

	switch(ClockSpeed) {

	  case Hz128 :
			clk_hertz = 128;
			break;

	  case Hz1024:
			clk_hertz = 1024;
			break;

	  case Hz4096:
			clk_hertz = 4096;
			break;
	  }


 // change interrupt vector

	OldRtcVec = _dos_getvect(RTC_VEC);
	_dos_setvect(RTC_VEC,RtcHook);

 // enable clock interrupt

	outp(IMR2,inp(IMR2) & RTC_MASK);
	outp(CMOSFLAG,STATUSA);

 // select clock frequency

	OldMaskA = inp(CMOSDATA);
	outp(CMOSFLAG,STATUSA);
      outp(CMOSDATA,OldMaskA & (ClockSpeed | 0xf0));

 // enable periodic interrupts

	outp(CMOSFLAG,STATUSB);
	OldMaskB = inp(CMOSDATA);
	outp(CMOSFLAG,STATUSB);
	outp(CMOSDATA, OldMaskB | RTC_FLAG);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void DisableRtcInts(void)
 {
	outp(CMOSFLAG,STATUSB);
	outp(CMOSDATA,OldMaskB);

	outp(CMOSFLAG,STATUSA);
	outp(CMOSDATA,OldMaskA);

	outp(IMR2,inp(IMR2) & !RTC_MASK);

	_dos_setvect(RTC_VEC,OldRtcVec);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 static void rtc_timer()
 {
      RtcCounter ++;

      if( RtcCounter == clk_hertz) {
            RtcCounter = 0;
            RtcSeconds++;
            }
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void rtc_open(void)
 {
      NewRtcVec = rtc_timer;        // set user function pointer
      RtcCounter = RtcSeconds = 0;  // initialize timers

      EnableRtcInts(Hz128);         // setup interrupt every 1/128 seconds
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void rtc_close(void)
 {
      DisableRtcInts();
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 unsigned long int rtctime(void)
 {
 unsigned long int Counter;

	  Counter = (long)(RtcSeconds);

      return(Counter);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 double rtctimed(void)
 {
 unsigned long int Counter;
 double Dcounter;

	  Counter = (long)(RtcSeconds) * (long)(clk_hertz) + (long)(RtcCounter);
	  Dcounter = (double)(Counter)/(double)(clk_hertz);
	  return(Dcounter);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 double rtc_tcks_per_second(void)
 {
      return(clk_hertz);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 double rtc_seconds_per_tick(void)
 {
      return(1.0/clk_hertz);
 }
