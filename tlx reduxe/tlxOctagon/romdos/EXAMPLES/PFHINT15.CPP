#pragma inline			// for the imbedded assembly

/* NOTE:
  This is an interrupt service routine.
  You can NOT compile this program with Test
  Stack Overflow turned on and get an
  executable file which will operate
  correctly.  Due to the nature of this
  function the formula used to compute
  the number of paragraphs may not
  necessarily work in all cases.  Use with
  care!  Terminate Stay Resident (TSR)
  programs are complex and no other support
  for them is provided.  Refer to the
  MS-DOS technical documentation
  for more information.  */

#include <dos.h>
#include <stdio.h>

#define INTR 0x15
#define itr_10 0x10

/* reduce heaplength and stacklength to make a smaller program in memory */
extern unsigned _heaplen = 1024;
extern unsigned _stklen  = 512;

void interrupt ( *oldhandler)(...);

void pChar(unsigned char mchar)
{
  _AH = 0xe;
  _AL = mchar;
  _BX = 0;
  geninterrupt(itr_10);
}

void pString(char *string)
{
  while (*string)
  {
    if (*string == '\\' )
    {
      string++;
      if (*string == 'n')
      {
	string++;
	pChar(0x0d); // Carriage Return
	pChar(0x0a); // Line Feed
      }
    }
    else
      pChar(*string++);
  }
}

void interrupt handler(...)
{
  unsigned int AXSave, DXSave;
  asm mov AXSave,AX
  asm mov DXSave,DX

  if ((AXSave == 0xAA55) && (DXSave == 0x1234))
    pString("Inside PFH int handler.\n");
  else
    oldhandler(); // does this pass ALL registers and allow return values
		  // to be returned from previous int 15 handlers???
}

int main(void)
{
 unsigned int fudge = 0x1000;
 printf("PFHINT15 - Power fail interrupt test program\n");

 oldhandler = _dos_getvect(INTR); // get old handler so we can call it

 _dos_setvect(INTR, handler);	 // install our handler

/* _psp is the starting address of the
   program in memory.  The top of the stack
   is the end of the program.  Using _SS and
   _SP together we can get the end of the
   stack.  You may want to allow a bit of
   saftey space to insure that enough room
   is being allocated ie:
   (_SS + ((_SP + safety space)/16) - _psp)
*/
 _dos_keep(0, (_SS + (_SP/16) - _psp) + fudge);


return 0;
}

