/* file: rtclock.h */

 #ifndef _RTCLOCK_H
 #define _RTCLOCK_H

 #define Hz128    2
 #define Hz1024   6
 #define Hz4096   4

 void EnableRtcInts(int ClockSpeed);
 void DisableRtcInts();

 extern  int RtcCounter;
 extern void (far *NewRtcVec)();

// hi-res timer interface standard

// #define HRTIME_TCKS_PER_SECOND 1193180.0
// #define HRTIME_SECONDS_PER_TICK 0.0000008380965152

// void hrt_open(void);
// void hrt_close(void);
// unsigned long int hrtime(void);

 void rtc_open(void);
 void rtc_close(void);
 unsigned long int rtctime(void);
 double rtctimed(void);

 double rtc_tcks_per_second(void);
 double rtc_seconds_per_tick(void);

#endif
