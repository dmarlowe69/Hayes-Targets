//+ Octagon Systems Corporation
//+ RS485TST source code - Copyright 1997
//+  Source  : RS485TST.CPP - RS485 test example program for 6000 series CPU
//+  Author  : H.K.Wong
//++ History : HK     03-18-97    created module

#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <bios.h>
#include <conio.h>
#include <ctype.h>

// Define constants here

#define GOOD 0
#define BAD  ~GOOD
#define TRUE 1

#define MENU_CYCLE 1
#define EXIT_CYCLE 2
#define INIT_CYCLE 10
#define PUT_CYCLE 11
#define GET_CYCLE 12
#define PUTnGET_CYCLE 13
#define PUT_LOOP_CYCLE 14
#define GET_LOOP_CYCLE 15
#define ROLL_CALL_CYCLE 16

#define SINGLE 0
#define LOOP (SINGLE+1)
#define SINGLEUSER (LOOP+1)

#define BUFFER_SIZE 256

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

// Define prototype here

int rs485GetCmd(int loopFlag);

// Define global variables here

static uchar oBuffer[BUFFER_SIZE];
static uchar iBuffer[BUFFER_SIZE];
static int ourId = 0xff;

static uint oBufferSegmentSave,oBufferOffsetSave;
static uint iBufferSegmentSave,iBufferOffsetSave;

static int nTimeSend = 0;
static int nTimeReceive = 0;

// Initialization command
// Call int 17h function 0fah, subfunction 00h

int rs485InitCmd(void)
{
  int status;

  asm {
	mov	ax,0fa00h
	mov	dx,0ffffh
	int	17h
	mov	status,ax
  }
  printf("\nINIT command status = %04x\n",status);
  return status;
}

// Set our id
// Call int 17h function 0fah, subfunction 08h

int rs485SetOurId(int id)
{
  asm {
	mov	ax,0fa08h
	mov	bx,id
	mov	dx,0ffffh
	int	17h
  }
  return 0;
}

// Get our id
// Call int 17h function 0fah, subfunction 07h

int rs485GetOurId()
{
  int id;

  asm {
	mov	ax,0fa07h
	mov	dx,0ffffh
	int	17h
	mov	id,ax
  }
  return id;
}

// Set input status
// Call int 17h function 0fah, subfunction 05h

int rs485GetIStatus()
{
  int status;
  int messageCount;

  asm {
	  mov	ax,0fa05h
	  mov	dx,0ffffh
	  int	17h
	  mov	status,ax
	  mov	messageCount,bx
  }
  return status;
}

// Get output status
// Call int 17h function 0fah, subfunction 06h

int rs485GetOStatus()
{
  int status;
  int messageCount;

  asm {
	  mov	ax,0fa06h
	  mov	dx,0ffffh
	  int	17h
	  mov	status,ax
	  mov	messageCount,bx
  }
  return status;
}

// Send
// Prepare what to send
// Call int 17h function 0fah, subfunction 01h

int rs485PutCmd(int loopFlag)
{
  int i, status;
  uchar c;
  uchar far *oPtr;
  uchar oTerm1 = 0x0d;
  uchar oTerm2 = '.';
  uint eventFlags;
  char userInput[20];

  do
  {
    for(int j=0;j<20;j++)
      userInput[j++] = '.';

    // Prepare messages

    if(loopFlag == SINGLEUSER)
    {
      printf("\nEnter message: ");
      scanf("%20s",userInput);
      printf("\n");
      strcpy(oBuffer, userInput);
      loopFlag = SINGLE;
    }
    else
    {
      // Send 8 different messages out

      nTimeSend++;
      i = nTimeSend%8;
      switch(i)
      {
	case 0:
	  strcpy(oBuffer,">02G113371\015");  // optomux: 02 reset
	  break;
	case 1:
	  strcpy(oBuffer,">01C2D6.");  // optomux: 01 turn-around delay 100ms
	  break;
	case 2:
	  strcpy(oBuffer,">FFACC.");  // octagon addon: ff are you there?
	  break;
	case 3:
	  strcpy(oBuffer,">FFC0533.");  //octagon addon: assign id 05 to ff
	  break;
	case 4:
	  strcpy(oBuffer,">05C0613.");  //octagon addon: assign id 06 to 05
	  break;
	case 5:
	  strcpy(oBuffer,">06C0716.");  //octagon addon: assign id 07 to 06
	  break;
	case 6:
	  strcpy(oBuffer,">FFC0533.");  //octagon addon assign id 05 to ff
	  break;
	case 7:
	  strcpy(oBuffer,">07C0516.");  //octagon addon assign id 05 to 07
	  break;
	default:
	  strcpy(oBuffer,">FFC0533.");  //octagon addon assign id 05 to ff
	  break;
      }
    }

    printf("\n\nSending (%d)...\n",nTimeSend);

    asm {
	  mov	ax,0fa01h
	  mov	es,oBufferSegmentSave
	  mov	bx,oBufferOffsetSave
	  mov	dx,0ffffh
	  int	17h
	  mov	status,ax
    }

    // Wait for message to be sent

    uint timeOut = 5000;		// 5000 ms max
    while(timeOut--)
    {
      if((eventFlags=rs485GetOStatus()) & 0x0001)
	break;
      delay(1);
    }

    status = (timeOut) ? GOOD : BAD;
    printf("SEND command status = %04x\n",eventFlags);

    if(status == GOOD)
    {
      oPtr = oBuffer;
      printf("[ ");
      do
      {
	c = *oPtr++;
      } while(c!=oTerm1 && c!=oTerm2 && printf("%c",c));
      printf(" ]");
    } // if status is good

    if(bioskey(1))
    {
      bioskey(0);
      loopFlag = SINGLE;
    }

    // Get response

    rs485GetCmd(SINGLE);

    if(loopFlag)
      delay(2000);
  } while (loopFlag);

  return status;
}

int itohex(uchar i, char *hexPtr)
{
  uchar hexHigh, hexLow;

  hexHigh = (uchar) (i >> 4 & 0x000f);
  if(hexHigh > 9)
    *hexPtr++ = 'A' - 10 + hexHigh;
  else
    *hexPtr++ = '0' + hexHigh;

  hexLow = (uchar) (i & 0x000f);
  if(hexLow > 9)
    *hexPtr = 'A' - 10 + hexLow;
  else
    *hexPtr = '0' + hexLow;
  return 0;
}

int hextoi(uchar hexChar)
{
  if (isxdigit(hexChar))
  {
    if (isdigit(hexChar))
      return (hexChar - '0');
    else
      return (toupper(hexChar) - 'A' + 10);
  }
  else
    return -1;
}

// Get message
// Call int 17h function 0fah, subfunction 02h

int rs485GetCmd(int loopFlag)
{
  int i, status;
  uchar c;
  uchar far *iPtr;
  uchar iTerm1 = 0x0d;
  uchar iTerm2 = '.';
  uchar csumMinus1, csumMinus2, checksum;
  uchar charMinus1, charMinus2;
  int stayInLoop, field;
  uchar idField, idField0, idField1, commandField;
  uint eventFlags;
  unsigned int timeOut;

  uint octagonChecksum, receivedChecksum, computedChecksum;

  do
  {
    ourId = rs485GetOurId();
    printf("\n\nID %02x is waiting to receive (%d)...\n",ourId,nTimeReceive++);

    status = 1;
    timeOut = 0x8000;
    while( (i=bioskey(1))==0 && status!=0 && timeOut--)
    {
      asm {
	    mov	ax,0fa02h
	    mov	es,iBufferSegmentSave
	    mov	bx,iBufferOffsetSave
	    mov	dx,0ffffh
	    mov	cx,0
	    int	17h
	    mov	status,ax
	  }
    }

    eventFlags=rs485GetIStatus() & 0x0002;

    if(i)
    {
      bioskey(0);
      loopFlag = SINGLE;
    }
    status = eventFlags ? GOOD : BAD;
    printf("RECEIVE command status = %04x\n",eventFlags);

    if(status == GOOD)
    {
      iPtr = iBuffer;

      stayInLoop = 1;
      field = 0;
      commandField = 0;
      checksum = 0;
      csumMinus1 = 0;
      csumMinus2 = 0;
      charMinus1 = 0;
      charMinus2 = 0;
      idField0 = 0;
      idField1 = 0;

      printf("{ ");
      do
      {
	c = *iPtr++;
	printf("%c",c);
	switch(c)
	{
	  case 0x0d:
	  case '.':
	  case 0x00:
	    printf(" }");
	    computedChecksum = csumMinus2 & 0xff;
	    receivedChecksum = hextoi(charMinus2)*16 + hextoi(charMinus1) & 0xff;
	    octagonChecksum = csumMinus2 + hextoi(idField0)*16 + hextoi(idField1) & 0xff;
	    printf("\nComputed checksum         = %02x",computedChecksum);
	    printf("\nReceived Checksum field   = %02x",receivedChecksum);
	    printf("\nComputed Octagon checksum = %02x",octagonChecksum);
	    printf("\nCommand   = %c",commandField);
	    printf("\nID code   = %c%c ",idField0,idField1);
	    if(ourId == ((hextoi(idField0)*16+hextoi(idField1))&0xff))
	    {
	      if(octagonChecksum == receivedChecksum ||
		 computedChecksum == receivedChecksum)
		printf("  ------------> My message (%02x)",ourId);
	      else
		printf("  ---->  My ID (%02x), but wrong checksum", ourId);
	    }
	    else
	      printf("  Not Mine");
	    stayInLoop = 0;
	    break;
	  case '>':
	    commandField = 0;
	    checksum = 0;
	    csumMinus1 = 0;
	    csumMinus2 = 0;
	    charMinus1 = 0;
	    charMinus2 = 0;
	    idField0 = 0;
	    idField1 = 0;
	    field = 1;
	    break;
	  default:
	    switch(field)
	    {
	      case 0:
		break;
	      case 1:
		idField0 = c; field++; break;
	      case 2:
		idField1 = c; field++; break;
	      case 3:
		commandField = c; field=0; break;
	      default:
		break;
	    }
	    csumMinus2 = csumMinus1;
	    csumMinus1 = checksum;
	    checksum = checksum + c;
	    charMinus2 = charMinus1;
	    charMinus1 = c;
	} // switch c
      } while (stayInLoop);
    } // if status is good
  } while (loopFlag);

  return status;
}

int rs485PutNGetCmd(void)
{
  printf("\nSend then Receive command....\n");
  ourId = rs485SetOurId(0x0000) & 0x00ff;
  rs485PutCmd(SINGLEUSER);
  rs485GetCmd(SINGLE);
  return 0;
}

int areYouThere(int id)
{
  uchar tempBuffer[40];
  int checksum;
  int status;

  strcpy(tempBuffer,">??A**.");
  itohex(id,tempBuffer+1);       // fill in ID field
  checksum = tempBuffer[1] + tempBuffer[2] + 'A' + id;
  itohex(checksum,tempBuffer+4);
  strcpy(oBuffer,tempBuffer);

  printf("\nSending roll call message: %s to %02x ... ",tempBuffer,id);

  asm {
	mov	ax,0fa01h
	mov	es,oBufferSegmentSave
	mov	bx,oBufferOffsetSave
	mov	dx,0ffffh
	int	17h
	mov	status,ax
      }

  return status;
}

int rollCall(int loopFlag)
{
  int i,j;
  int timeOut;
  uint status;
  uint eventFlags;
  uchar far *iPtr;

  while (loopFlag)
  {
    for (i=1; i<=33; i++)
    {
      if(i==33)
	i = 0xff;
      areYouThere(i);

      timeOut = 500; // 500ms max
      status = 1;

      for(j=0,iPtr=iBuffer;j<BUFFER_SIZE;j++)
	*iPtr++ = 0;

      while(timeOut-- && status != 0)
      {
	asm {
	      mov	ax,0fa02h
	      mov	es,iBufferSegmentSave
	      mov	bx,iBufferOffsetSave
	      mov	dx,0ffffh
	      mov	cx,0
	      int	17h
	      mov	status,ax
	    }

	if(status == 0)
	{
	  printf("ID %2x is present. (%s)",i,iBuffer);
	  break;  // break out of while loop
	}
	else
	  delay(1);
      } // while
      eventFlags=rs485GetIStatus() & 0x0002;
    } // for

    if(bioskey(1))
    {
      bioskey(0);
      loopFlag = SINGLE;
    }
    delay(500);
  } //while loopFlag

  return 0;
}

int main(void)
{
  int c;
  uint segSave,offSave;
  uchar far *debugBufferPtr;
  int i;
  uint stayInLoop = TRUE;
  int cycle = INIT_CYCLE;
  uint status = 0;

  printf("\n\
RS485TST Ver.1.00            Copyright 1996         Octagon Systems Corporation\n\
-------------------------------------------------------------------------------\n\n");

  asm {
	mov	ax,0fd00h
	mov	dx,0ffffh
	mov	cl,0fah
	int	17h
	mov	status,ax
  }

  if(status != 0x1234)
  {
    printf("ERROR: RS485 Driver not present!\n");
    return 1;
  }

  oBufferSegmentSave = FP_SEG((void far *)&oBuffer);
  oBufferOffsetSave = FP_OFF((void far *)&oBuffer);

  iBufferSegmentSave = FP_SEG((void far *)&iBuffer);
  iBufferOffsetSave = FP_OFF((void far *)&iBuffer);

  status = 0;
  while(stayInLoop == TRUE)
  {
    switch(cycle)
    {
      case INIT_CYCLE:
	status = rs485InitCmd();
	ourId = rs485GetOurId();
	cycle = (status != 0) ? EXIT_CYCLE : MENU_CYCLE;
	break;

      case MENU_CYCLE:
	status = 0;
	printf("\n\n Enter:");
	printf("\n   1 - INITIALIZATION      5 - SEND ONLY LOOP");
	printf("\n   2 - SEND ONLY           6 - ROLL CALL");
	printf("\n   3 - RECEIVE ONLY        7 - RECEIVE ONLY LOOP");
	printf("\n   4 - SEND AND RECEIVE\n\n");
	printf(  " ESC - Exit\n\n");
	c = bioskey(0) & 0xff;
	switch(c)
	{
	  case '1':
	    cycle = INIT_CYCLE;
	    break;

	  case '2':
	    cycle = PUT_CYCLE;
	    break;

	  case '3':
	    cycle = GET_CYCLE;
	    break;

	  case '4':
	    cycle = PUTnGET_CYCLE;
	    break;

	  case '5':
	    ourId = rs485SetOurId(0x0000);
	    cycle = PUT_LOOP_CYCLE;
	    break;

	  case '6':
	    ourId = rs485SetOurId(0x0000);
	    cycle = ROLL_CALL_CYCLE;
	    break;

	  case '7':
	    ourId = rs485SetOurId(0x00ff);
	    printf("\n\nID %02x is waiting to receive ...\n",ourId);
	    cycle = GET_LOOP_CYCLE;
	    break;

	  case 0x1b:					/* ESC */
	    cycle = EXIT_CYCLE;
	    break;

	  default:
	    break;
	}
	break;

      case PUT_CYCLE:
	status = rs485PutCmd(SINGLEUSER);
	cycle = MENU_CYCLE;
	break;

      case GET_CYCLE:
	status = rs485GetCmd(SINGLE);
	cycle = MENU_CYCLE;
	break;

      case PUT_LOOP_CYCLE:
	ourId = rs485SetOurId(0x0000) & 0x00ff;  //hkdebug new
	status = rs485PutCmd(LOOP);
	cycle = MENU_CYCLE;
	break;

      case GET_LOOP_CYCLE:
	status = rs485GetCmd(LOOP);
	cycle = MENU_CYCLE;
	break;

      case ROLL_CALL_CYCLE:
	status = rollCall(LOOP);
	cycle = MENU_CYCLE;
        break;

      case PUTnGET_CYCLE:
	status = rs485PutNGetCmd();
	cycle = MENU_CYCLE;
	break;

      case EXIT_CYCLE:
      default:
	switch(status)
	{
	  case 0:
	    printf("Exit RS485 Test : STATUS = OK\n");
	    break;

	  default:
	    printf("Exit RS485 Test : STATUS = BAD\n");
	    break;
	}
	stayInLoop = ~TRUE;
    }
  }
}

