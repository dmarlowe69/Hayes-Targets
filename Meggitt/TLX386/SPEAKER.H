/* file: speaker.c */

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

 extern char song[];
 extern char *songpointer;
 extern int octave;
 extern int duration;
 extern int pitch;
 extern int songindex;

 extern void (interrupt far *funptr_sound)(void);	/* pointer to function */

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
