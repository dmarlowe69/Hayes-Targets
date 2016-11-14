/* Example program to demonstrate wake up options on IRQ8 (CMOS alarm)
   and try to do something.

   This code written and compiled in Borland C/C++ v3.1

   This assumes the CMOS clock is in 24 hour mode and BCD format */


#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <dos.h>

/* this routine reads a value from standard CMOS */
int readCMOS(int cmosreg)
{
  outportb(0x70,cmosreg);
  return (inportb(0x71));
}

/* this routine writes a value to standard CMOS */
void writeCMOS(int cmosreg, int cmosvalue)
{
  outportb(0x70,cmosreg);
  outportb(0x71,cmosvalue);
}

int BCDtoBIN(int value)
{
  return (((value & 0xf0) >> 4) * 10) + (value & 0xf);
}

int BINtoBCD(int value)
{
  return (((value / 10) << 4) + (value % 10));
}


/* The CMOS RT clock interrupt (also known as IRQ8) */
#define IRQ8 0x70

#ifdef __cplusplus
    #define __CPPARGS ...
#else
    #define __CPPARGS
#endif

void interrupt (*oldhandler)(__CPPARGS);

int alarmHappened = 0;

void interrupt handler(__CPPARGS)
{
   if (!(readCMOS(0x0c) & 0x20))
     oldhandler(); /* call old interrupt handler if required */
   else
   {
     alarmHappened =  1;  /* set happened flag */
     outportb(0xa0,0x20); /* acknowledge interrupt to PIC2 */
     outportb(0x20,0x20); /* acknowlegde interrupt to PIC1 */
   }
}

int prevPIC2;

void installInt(void)
{
  disable();

  prevPIC2 = inportb(0xa1);
  outportb(0xa1,prevPIC2 & 0xfe);

  /* get the address of the current IRQ8 interrupt */
  oldhandler = getvect(IRQ8);

  /* install the new interrupt handler */
  setvect(IRQ8, handler);

  enable();
}

void removeInt(void)
{
  disable();
  /* install the previous interrupt handler */
  setvect(IRQ8, oldhandler);

  outportb(0xa1,prevPIC2);
  enable();

}

/* now for the Power management code */

#define EnterSuspend    0x85
#define PDsio           0x86
#define PDfloppy        0x87
#define PDhdd           0x88
#define PDserialPortA   0x89
#define PDserialPortB   0x8A
#define PDparallelPort  0x8B
#define PUparallelPort  0x8C

void doAPMint(unsigned char apmINT)
{
  _AH = 0x53;
  _AL = 0x80;
  _BH = apmINT;
  geninterrupt(0x15);
}

/* this routine reads the current CMOS time */
/* converting it from BCD to decimal in the process */
void getCMOSTime(int *hour, int *minute, int *second)
{
  *second = BCDtoBIN(readCMOS(0x00));
  *minute = BCDtoBIN(readCMOS(0x02));
  *hour   = BCDtoBIN(readCMOS(0x04));
}

/* this routine sets the CMOS alarm time */
/* The time is first checked for wrapping values (ie. minutes > 59) */
/* and adjusts other values accordingly. It then converts to BCD */
/* which the CMOS alarm requires */
int setCMOSAlarm(int *hour, int *minute, int *second)
{
  int temp;

  temp = readCMOS(0x0b);	/* read the CMOS status register B */
  if (temp & 0x40)		/* Check if periodic int already set */
    return (0);		        /* then return error */

  while (*second >= 60)
  {
    *second = *second - 60;
    *minute = *minute + 1;
  }

  while (*minute >= 60)
  {
    *minute = *minute - 60;
    *hour = *hour + 1;
  }

  if (*hour >= 24)
    *hour = *hour - 24;

  writeCMOS(0x01,BINtoBCD(*second));
  writeCMOS(0x03,BINtoBCD(*minute));
  writeCMOS(0x05,BINtoBCD(*hour));

  temp = readCMOS(0x0b);	/* read the CMOS status register B */
  temp = temp | 0x20;		/* set the Enable alarm interrupt flag */
  writeCMOS(0x0b,temp);		/* enable the alarm interrupt */

  return (1);
}

void main()
{
  int hour, minute, second;

  printf("WAKEIRQ8 V1.01 Borland C Demonstration program.\n\n");
  printf("Before this program will execute correctly, the following flags\n");
  printf("must be set using the PMISETUP setup program:\n");
  printf("    PMI ENABLE = Y\n");
  printf("    IRQ8 RESUME = Y\n");

  printf("To do this a sample file WAKEIRQ8.PMI is included in the examples\n");
  printf("subdirectory. To set these parameters execute the following\n");
  printf("command:\n");
  printf("   PMISETUP WAKEIRQ8.PMI\n");
  printf("Then press reset and run this program.\n\n");

  printf("This example program sets the alarm time to interrupt after\n");
  printf("30 seconds and then enters suspend mode. In 30 seconds, the\n");
  printf("alarm (IRQ8) occurs and resumes the system. If a key is pressed\n");
  printf("the system exits the example program.\n");

  printf("Press any key to continue example or ESC to exit.\n");
  while (kbhit() == 0);
  if (getch() == 0x1b)
    return;

  printf("\n\nPress any key to stop example.\n");

  installInt(); /* Install the IRQ8 interrupt */

  getCMOSTime(&hour,&minute,&second);

  printf("\n\nThe current time is %02d:%02d:%02d\n",hour,minute,second);

  while (1) /* outside loop where we do something */
  {
    second = second + 30; /* Do not set this too short, some resumes take
			     a little while to do (like HDD resumes) */

    if (setCMOSAlarm(&hour,&minute,&second))
    {
      printf("Setting CMOS alarm (IRQ8) to %02d:%02d:%02d\n\n",
	     hour,minute,second);
      delay (3000); /* Delay 3 seconds so user can see shutdown time */
    }
    else
    {
      printf("CMOS alarm/periodic interrupt currently set.\n");
      return;
    }

    /* power down devices that we don't care about */
    /* Your application may be different */
    doAPMint(PDfloppy);
    doAPMint(PDhdd);
    doAPMint(PDserialPortA);
    doAPMint(PDserialPortB);
    doAPMint(PDparallelPort);

    /* enter suspend */
    doAPMint(EnterSuspend);

    /* Check if it was IRQ 8 to give a reason for wakeup */
    if (alarmHappened)
      printf("Resumed by IRQ8\n");
    else
      printf("Resumed by non-IRQ8 interrupt.\n");

    /* When we get here we have woken up from suspend */
    /* Presumably from the ALARM, but it could be from any of */
    /* the other enabled resume IRQs (keyboard etc.) */
    /* We may want to check and make sure we are at the correct time */

    /* power up the parallel port (if needed) */
    doAPMint(PUparallelPort);

    /* The user pressed a key so lets ask them what they want to do */
    if (kbhit())
    {
      printf("WAKEIRQ8 example terminated due to keystroke entered.\n");
      getche(); /* read the key */
      break;
    }
  }

  removeInt(); /* Remove the IRQ8 interrupt */
}

