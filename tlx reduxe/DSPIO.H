//	file: dspio.h

//////////////////////////////////////////////////////////////////////////////
//
//      DSPIO header file
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      UART general defintions
//

 #define TRUE  1
 #define ERR  -1

//
//  UART function prototypes
//

 void dspio_1_open(unsigned baud_rate);	/* initialize UART */
 void dspio_1_close(void);				/* close out uart */
 int dspio_1_check(void);				/* check for character */
 int dspio_1_getc(void);				/* get charcter */
 int dspio_1_poll(void);				/* wait for character */
 void dspio_1_flush(void);				/* flush characters from receive buffer */
 void dspio_1_putc(int ch);				/* output character to UART */
 void dspio_1_gets(char *str);			/* input string from receive buffer */
 void dspio_1_puts(char *str);			/* output string to uart */

 void dspio_2_open(unsigned baud_rate);	/* initialize UART */
 void dspio_2_close(void);				/* close out uart */
 int dspio_2_check(void);				/* check for character */
 int dspio_2_getc(void);				/* get charcter */
 int dspio_2_poll(void);				/* wait for character */
 void dspio_2_flush(void);				/* flush characters from receive buffer */
 void dspio_2_putc(int ch);				/* output character to UART */
 void dspio_2_gets(char *str);			/* input string from receive buffer */
 void dspio_2_puts(char *str);			/* output string to uart */

 static int dspio_makebaud(unsigned baud);

//
//  lpt function prototypes
//

 int dspio_init_port(void);
 int dspio_assign_lpt_port(unsigned int port);

 void dspio_outp(unsigned char);
 unsigned char dspio_inp(void);

 void dspio_clear_all_bits(void);
 void dspio_set_all_bits(void);
 void dspio_set_bit(unsigned char bit0_7);
 void dspio_clear_bit(unsigned char bit0_7);
 void dspio_set_bit_pattern(unsigned char pattern);
 void dspio_set_masked_bits(unsigned char mask);
 void dspio_clear_masked_bits(unsigned char mask);
 unsigned char dspio_read_output_byte(void);
 unsigned char dspio_read_output_bit(unsigned char bit0_7);
 unsigned char dspio_input_byte(void);
 unsigned char dspio_input_bit(unsigned char bit0_7);

 int dspio_toggle_bit(int bit0_7);

//
//      ROMable function prototypes
//

 extern void (*dspio_printfptr)(char *string);      /* function pointer */
 void dspio_printf_init(void (*ptr)(char *string));

 int dspio_printf(char const *,...);                /* same as microsoft function */
