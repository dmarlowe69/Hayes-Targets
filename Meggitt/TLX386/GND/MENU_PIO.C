/* file: menu_pio.c */

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include	<dos.h>
 #include	<conio.h>

 #include	<hayes.h>
 //#include	<gnd.h>

 #include	"ground.h"
 #include	"menu_utl.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 void pio_menu(void)
 {
 int i;
 char choice = 'x';
 char temp[80];

	com_puts("\r\n\r\nPIO FUNCTION MENU\r\n");

	while (toupper(choice) != 'Q') {
		com_puts("\r\nINPUTS ARE UPDATED WITH EACH CHOICE\r\n");
		printf("\r\n Input byte = %02x\t\t\t Output byte = %02x\r\n",dspio_input_byte(), dspio_read_output_byte());
		printf("\r\n Input bit: 7 6 5 4 3 2 1 0\t\t Output bit: 7 6 5 4 3 2 1 0\r\n Currently:");

		for (i=7; i>=0; i--)
			printf(" %d", (int)dspio_input_bit((unsigned char)i));
		printf("\t\t Currently: ");
		for (i=7; i>=0; i--)
			printf(" %d", (int)dspio_read_output_bit((unsigned char)i));

        com_puts("\r\n\r\n\r\nOUTPUT CHOICES\r\n");
        com_puts(" 1 Clear all bits\r\n");
        com_puts(" 2 Clear single bit\r\n");
        com_puts(" 3 Clear masked bits\r\n");
        com_puts(" 4 Set all bits\r\n");
        com_puts(" 5 Set single bit\r\n");
        com_puts(" 6 Set masked bits\r\n");
        com_puts(" 7 Set bit pattern\r\n");

        com_puts("\r\n Enter Choice number, or Q to quit: ");
		choice = toupper(com_getc());

		switch(choice) {

		case '1':
			dspio_clear_all_bits();
			break;

		case '2':
            printf("\r\nClear which bit (0-7)? ");
			//scanf("%d",&i);
			com_gets(temp);
			i = atoi(temp) & 0x0f;
			dspio_clear_bit((unsigned char)i);
			break;

		case '3':
			//printf("\nEnter mask byte (hex) to clear (1's cleared, 0's unchanged): ");
            printf("\r\nEnter mask byte (dec) to clear (1's cleared, 0's unchanged): ");
			//scanf("%x",&i);
			com_gets(temp);
			i = atoi(temp) & 0x0f;
			dspio_clear_masked_bits((unsigned char)i);
			break;

		case '4':
			dspio_set_all_bits();
			break;

		case '5':
            printf("\r\nSet which bit (0-7)? ");
			//scanf("%d",&i);
			com_gets(temp);
			i = atoi(temp) & 0x0f;
			dspio_set_bit((unsigned char)i);
			break;

		case '6':
			//printf("\nEnter mask byte (hex) to set (1's set, 0's unchanged): ");
            printf("\r\nEnter mask byte (dec) to set (1's set, 0's unchanged): ");
			//scanf("%x",&i);
			com_gets(temp);
			i = atoi(temp) & 0x0f;
			dspio_set_masked_bits((unsigned char)i);
			break;

		case '7':
			//printf("\nEnter pattern byte (hex) to output: ");
            printf("\r\nEnter pattern byte (dec) to output: ");
			//scanf("%x",&i);
			com_gets(temp);
			i = atoi(temp) & 0x0f;
			dspio_set_bit_pattern((unsigned char)i);
			break;

		} /* switch i */

	} /* while(choice) */

} /* menu_pio() */
