/* file: speaker.c */

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #define CTC_PORT		 0x40
 #define CTC_DATA_0      0x40
 #define CTC_DATA_1      0x41
 #define CTC_DATA_2      0x42
 #define CTC_CMD_PORT    0x43
 #define CTC_LATCH       0x00
 #define CONTROL_PORT    0x61

 typedef struct {
	int pitch;
	char name[3];
	} NOTE;

 NOTE scale[]={	262,"c",
				278,"c#",
				294,"d",
				312,"d#",
				330,"e",
				349,"f",
				371,"f#",
				392,"g",
				416,"g#",
				440,"a",
				468,"a#",
				494,"b"
				};

 #define	SCALESIZE	(sizeof(scale)/sizeof(NOTE))

/* this is blue peter, a tradional hornpipe */
 char song[]="o3 c16 o2 b16 o3 c16 r16 o2 c8 c8 g16 f16 e16 g16 o3 c8 c16 e16 "
	    "d16 c16 d16 r16 o2 d8 d8 d16 c16 o1 b16 o2 d16 g8 g8 "
	    "o2 a16 b16 o3 c16 o2 b16 "
	    "a16 g16 a16 g16 "
	    "f16 e16 f16 e16 "
	    "d16 c16 d16 c16 "
	    "o1 b16 a16 g16 a16 "
	    "b16 o2 c16 d16 e16 "
	    "f16 g16 e8 c8 c8 "
	    "o3 c16 o2 b16 o3 c16 r16 c8 c8 "
	    "c16 o2 b16 a16 r16 f8 f8 "
	    "b16 o3 c16 d16 r16 d8 d8 d16 c16 o2 b16 r16 o2 g8 g8 "
	    "o2 a16 b16 o3 c16 o2 b16 "
	    "a16 o2 g16 a16 g16 "
	    "f16 e16 f16 e16 "
	    "d16 c16 d16 c16 "
	    "o1 b16 a16 g16 a16 b16 o2 "
	    "c16 d16 e16 f16 g16 e8 c8 c8"
	    ;

 void sound_main(void);
 void sound_beeps(int beeps);
 void sound_beep(void);
 void init_sound(unsigned frequency);
 void adjust_sound(unsigned frequency);
 void adjust_sound_union(unsigned frequency);
 void sound_on(void);
 void sound_off(void);
 void sound_delay(unsigned ms_count);
 //void timed_delay(unsigned count);
 int timed_delay(unsigned count);
 unsigned get_count(void);
 void counter_init(void);

/****************************************************************************
 *
 *	sound globals
 *
 ***************************************************************************/

 char *songpointer=NULL;
 int octave=1;
 int duration=4;
 int pitch=-1;
 int songindex=0;

 void (interrupt far *funptr_sound)(void);	/* pointer to function */

 void main_playsong(void);
 void playtune(char *song);
 void kill_playtune(void);
 void interrupt far beNoisy(void);
 void DoSomething(void);

 void beNoisy_fun(void);

/****************************************************************************
 *
 *	sound functions
 *
 ***************************************************************************/

 void main_laser(void);
 void laser(void);

 void main_siren(void);
 void siren(void);

 void main_music(void);
 void tone(int frequency);

 void main_hearing(void);
 void hear_tone(int frequency);

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 void sound_main(void)
 {
 unsigned x;

	x = 100;
	init_sound(x);
	sound_on();
	//while(1)
	//{
		//while(x < 10000)
		while(x < 5000)
		{
		   adjust_sound(x);
		   x = x + 1;
		   sound_delay(1);
		}
		while(x > 100)
		{
		   adjust_sound(x);
		   x = x - 1;
		   sound_delay(1);
		}
	//}
	sound_off();
 }

 void sound_beeps(int beeps)
 {
	while(beeps--) {
		sound_beep();
		sound_delay(100);
	}

 }

 void sound_beep(void)
 {
	sound_on();
	//init_sound(523);
	init_sound(scale[0].pitch);
	sound_delay(100);
	sound_off();
 }

 void init_sound(unsigned frequency)
 {
 unsigned divisor;

	divisor = (unsigned)(1193181l / frequency);

	outp(CTC_CMD_PORT,0xb6);
	outp(CTC_DATA_2,divisor & 0xff);
	outp(CTC_DATA_2,divisor >> 8);
 }

 void adjust_sound(unsigned frequency)
 {
 unsigned divisor;

	divisor = (unsigned)(1193181l / frequency);

	outp(CTC_CMD_PORT,0xb6);
	outp(CTC_DATA_2,divisor & 0xff);
	outp(CTC_DATA_2,divisor >> 8);
 }

 void adjust_sound_union(unsigned frequency)
 {
 union {
	unsigned divisor;
	unsigned char c[2];
	} count;

	count.divisor = (unsigned)(1193181l / frequency);

	outp(CTC_CMD_PORT,0xb6);
	outp(CTC_DATA_2, count.c[0]); /* send low-order byte */
	outp(CTC_DATA_2, count.c[1]); /* send high-order byte */
 }

 void sound_on(void)
 {
	outp(0x61,inp(0x61) | 3);
 }

 void sound_off(void)
 {
	outp(0x61,inp(0x61) & 0xfc);
 }

//
// This function is based upon the known clock rate of 1.19MHz. used by the
// timed_delay function. Calling timed_delay with an argument of 1193 results
// in a delay of 1mS. To allow larger delays this functio will call timed_delay
// an appropriate number of times for the millisecond delay required
//
 void sound_delay(unsigned ms_count)
 {
 unsigned x;

        for(x=0; x < ms_count; x++)
                timed_delay(1193);
 }

//
// This function provides a semi-precision delay that is not dependent upon
// processor speed. It uses counter timer channel 0 to guage elapsed time.
//
 int timed_delay(unsigned count)
 {
 unsigned start_time, current_time;

	start_time = get_count();	   /* Get the starting count in ticks */

	while(1)
	{
		current_time = get_count();	   /* get current count */
		if((start_time - current_time) >= count)
			break;	/* if enough ticks have occured, get out */
	}
	return 0;
 }

//
// This function uses the counter latch command to freeze the current
// count value so that it may be read. The value is then returned.
//
 unsigned get_count(void)
 {
 unsigned lsb_val,msb_val;
 unsigned total_count;

	outp(CTC_CMD_PORT,CTC_LATCH);			/* Latch count */
	lsb_val = inp(CTC_DATA_0) & 0xff;	   /* Get lsb of count */
	msb_val = inp(CTC_DATA_0) & 0xff;	   /* Get msb of count */
	total_count = (msb_val << 8) | lsb_val;
	return total_count;
 }

/* this function sets up the counter/timer channel 0 to count continously.
   The input frequency is about 1.19Mhz so each count will be worth about
   .84uS
*/

 void counter_init(void)
 {
	outp(CTC_CMD_PORT,0x30);	   /* counter 0, mode 3 */
	outp(CTC_DATA_0, 0xff);		   /* Maximum count */
	outp(CTC_DATA_0, 0xff);
 }

/****************************************************************************
 *
 *	sound functions
 *
 ***************************************************************************/

 #define LASER_DELAY 5000
 #define LASER_RATE 50

/* Test	a laser blast effect. */
 void main_laser(void)
 {
	laser();
	laser();
	laser();
	laser();
	laser();
 }

/* Create a laser blast effect. */
 void laser(void)
 {
 unsigned frequency;
 unsigned long i;

	sound_on();
	for(frequency = 3000; frequency>1000; frequency-=LASER_RATE) {
		adjust_sound(frequency);
		for(i=0; i<LASER_DELAY; ++i) ;
	}
	sound_off();
 }

 void main_siren(void);
 void siren(void);

 #define SIREN_DELAY 10000
 #define SIREN_RATE 10

/* test	a siren effect. */
 void main_siren(void)
 {
	siren();
 }

/* Create a siren effect. */
 void siren(void)
 {
 unsigned frequency;
 unsigned long i;

	sound_on();
	/* ascending siren */
	for(frequency=1000; frequency<3000; frequency+=SIREN_RATE) {

		adjust_sound(frequency);

		for(i=0; i<SIREN_DELAY; ++i) ;
	}
  
/* descending siren */
	for( ; frequency>1000; frequency-=SIREN_RATE) {

		adjust_sound(frequency);

		for(i=0; i<SIREN_DELAY; ++i) ;
	}
	sound_off();
 }

 #define MUSIC_DELAY 64000

 #ifdef _UNDER_DOS
 void main_music(void)
 {
  int frequency;

	do {
		do {
		frequency = rand();
		} while(frequency>5000); /* alter to your personal taste */
		tone(frequency);
	} while(!kbhit());
   
 }
 #endif

/* Beep the speaker using the specified frequency. */
 void tone(int frequency)
 {
 unsigned long i;

	adjust_sound(frequency);

	sound_on();

	for(i=0; i<MUSIC_DELAY; ++i) ;

	sound_off();
 }

 #define HEARING_DELAY 100000

 #ifdef _UNDER_DOS
 void main_hearing(void)
 {
 int frequency;

	do {
		printf("enter frequency (0 to exit): ");
		scanf("%d", &frequency);
		if(frequency) tone(frequency);
	} while(frequency);
 }
 #endif

/* Beep the speaker using the specified frequency. */
 void hear_tone(int frequency)
 {
 unsigned long i;

	adjust_sound(frequency);

	sound_on();

	for(i=0; i<HEARING_DELAY; ++i) ;

	sound_off();
 }

 #ifdef _UNDER_DOS
 void main_playsong(void)
 {
	funptr_sound=getvect(0x1c);
	_dos_setvect(0x1c,beNoisy);

	playtune(song);

	DoSomething();

	dos_setvect(0x1c,funptr_sound);
	sound_off();
 }
 #endif

 void playtune(char *song)
 {
	_disable();
	songindex=0;
	songpointer=song;
	_enable();
 }

 void kill_playtune(void)
 {
	_disable();
	songindex=0;
    songpointer=NULL;
	_enable();
 }

 void interrupt far beNoisy(void)           /* interrupt function */
 {
 static int count,busy;
 int i,n,gotnote=0;
 int j;

	if(busy) goto exitisr;

	if(songpointer == NULL) goto exitisr;

	if(count > 1) {
		--count;
		goto exitisr;
	}

	busy=1;

	do {
		while(songpointer[songindex]==32) ++songindex;

		switch(n=tolower(songpointer[songindex])) {
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
				for(i=0;i<SCALESIZE;++i) {
					if(n==scale[i].name[0]) {
						++songindex;
						if(songpointer[songindex]=='#') {
							++i;
							++songindex;
						}
						pitch=scale[i].pitch;
						break;
					}
				}

				n=atoi(songpointer+songindex);
				if(n >= 1 && n <= 32) duration=32/n;
				++songindex;
				gotnote=1;
				break;
			case 'r':
				++songindex;
				n=atoi(songpointer+songindex);
				if(n >= 1 && n <= 32) duration=32/n;
				pitch=-1;
				++songindex;
				gotnote=1;
				break;
			case 'o':
				++songindex;
				n=atoi(songpointer+songindex);
				if(n >= 1 && n <= 4) octave=n;
				pitch=-1;
				++songindex;
				break;
			case 0:
				songindex=0;
				pitch=-1;
				duration=1;
				octave=1;
				gotnote=1;
				break;
			default:
				while(songpointer[songindex] != 32 &&
				      songpointer[songindex] != 0) ++songindex;
				break;
		}
	} while (!gotnote);

	if(pitch==-1) sound_off();
	else {
		sound_off();
		adjust_sound(pitch*(1<<(octave-1)));
		sound_on();
	}
	count=duration;
	busy=0;

 exitisr:
	j=0;
 }

 #ifdef _UNDER_DOS
 void DoSomething(void)
 {
	do {
	} while(!kbhit());
	getch();

 }
 #endif

 void beNoisy_fun(void)
 {
 static int count,busy;
 int i,n,gotnote=0;
 int j;

	if(busy) goto exitfun;

	if(songpointer == NULL) goto exitfun;

	if(count > 1) {
		--count;
		goto exitfun;
	}

	busy=1;

	do {
		while(songpointer[songindex]==32) ++songindex;

		switch(n=tolower(songpointer[songindex])) {
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
				for(i=0;i<SCALESIZE;++i) {
					if(n==scale[i].name[0]) {
						++songindex;
						if(songpointer[songindex]=='#') {
							++i;
							++songindex;
						}
						pitch=scale[i].pitch;
						break;
					}
				}

				n=atoi(songpointer+songindex);
				if(n >= 1 && n <= 32) duration=32/n;
				++songindex;
				gotnote=1;
				break;
			case 'r':
				++songindex;
				n=atoi(songpointer+songindex);
				if(n >= 1 && n <= 32) duration=32/n;
				pitch=-1;
				++songindex;
				gotnote=1;
				break;
			case 'o':
				++songindex;
				n=atoi(songpointer+songindex);
				if(n >= 1 && n <= 4) octave=n;
				pitch=-1;
				++songindex;
				break;
			case 0:
				songindex=0;
				pitch=-1;
				duration=1;
				octave=1;
				gotnote=1;
				break;
			default:
				while(songpointer[songindex] != 32 &&
				      songpointer[songindex] != 0) ++songindex;
				break;
		}
	} while (!gotnote);

	if(pitch==-1) sound_off();
	else {
		sound_off();
		adjust_sound(pitch*(1<<(octave-1)));
		sound_on();
	}
	count=duration;
	busy=0;

 exitfun:
	j=0;
 }
