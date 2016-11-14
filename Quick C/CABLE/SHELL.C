#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

#include	<io.h>	// for _mktemp

#include "swap.h"

 int shell(void)
 {
 int swap_return;
 unsigned char exec_return;
 unsigned char *comspec;
 unsigned int dos_ptr;
 char swap_file[] = "SWXXXXXX";

   if (!xms_installed())
      printf ("No ");
   printf ("XMS driver detected.\n");

   if (!ems4_installed())
      printf ("No ");
   printf ("EMS 4.0 driver detected.\n");

/* Try to make temporary name for directory. */
	if( _mktemp( swap_file ) == NULL ) {
		perror( "Can't make temporary file" );
		exit( 1 );
	}
   printf ("Temporary file %s selected\n",swap_file);

   printf ("\n** Type EXIT to return to SWAPTEST **\n");

   comspec = getenv ("COMSPEC");

//	swap_return = swap (comspec, "", &exec_return, "swaptest.fil");
   swap_return = swap (comspec, "", &exec_return, swap_file);

   printf ("\n\nBack in SWAPTEST now.\n\n");

   switch (swap_return)
      {
      case SWAP_OK:        printf ("Successful, executed program returned %d.\n", (int)exec_return);
                           break;

      case SWAP_NO_SHRINK: printf ("Unable to shrink DOS memory block.\n");
                           break;

      case SWAP_NO_SAVE:   printf ("Unable to save program to memory or disk.\n");
                           break;

      case SWAP_NO_EXEC:   printf ("DOS EXEC call failed.  Error is %d: ", (int)exec_return);
                           switch (exec_return)
                              {
                              case BAD_FUNC:       printf ("Bad function.\n");                        break;
                              case FILE_NOT_FOUND: printf ("Program file not found.\n");              break;
                              case ACCESS_DENIED:  printf ("Access to program file denied.\n");       break;
                              case NO_MEMORY:      printf ("Insufficient memory to run program.\n");  break;
                              case BAD_ENVIRON:    printf ("Bad environment.\n");                     break;
                              case BAD_FORMAT:     printf ("Bad format.\n");                          break;
                              default:             printf ("Unexpected error code #%d (decimal).\n", (int)exec_return);
                                                   printf ("Consult DOS technical reference manual.\n");
                                                   break;

                              }
      }
   printf("press any key");
   getch();
   return (0);

}
