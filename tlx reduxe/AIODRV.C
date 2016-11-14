//  file: AIODRV.C

//////////////////////////////////////////////////////////////////////////////
//
//      Analog I/O functions
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      Include definitions
//

 #include       <stdio.h>
 #include       <dos.h>
 #include       <conio.h>

 #include       "config.h"

//
//      I/O addresses of AIO control registers
//

 #define AIO_BASE 0x0300            // base address of I/O board

//
//      AIO_STATUS Defintions
//
 signed int aio_status;

//
//      General Defintions
//
 #define TIME_OUT 65536
 #define INVALID  65536
 #define AIO_OK 	0
 #define AIO_ERR    1

//
//      Function Prototypes
//
 void Lamp_ON(void);
 void Lamp_OFF(void);
 void AUX_ON(void);
 void AUX_OFF(void);

 void A_D_Init(void);
 unsigned int Read_Thermouple();
 unsigned int Read_Alternator_Prior_to_Lamp();
 unsigned int Read_Pump();
 unsigned int Read_Battery_Voltage();
 unsigned int Read_Modem_Signal_Strenght();
 unsigned int Read_Radar_Altitude();
 unsigned int Read_Alternator_Voltage();
 unsigned long Read_A_D(unsigned char channel);

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void Lamp_ON(void)
 {
    outp(AIO_BASE+5,0x4f);  //01001111b
    outp(AIO_BASE+4,0xff);  //11111111b
    inp(AIO_BASE+5);
 }
//
//
//
 void Aux_ON(void)
 {
    outp(AIO_BASE+5,0xcf);  //11001111b
    outp(AIO_BASE+4,0xff);  //11111111b
    inp(AIO_BASE+5);
 }
//
//
//
 void Lamp_OFF(void)
 {
    outp(AIO_BASE+5,0x40); //01000000b
    outp(AIO_BASE+4,0x00); //00000000b
    inp(AIO_BASE+5);
 }
//
//
//
 void Aux_OFF(void)
 {
    outp(AIO_BASE+5,0xc0);  //11000000b
    outp(AIO_BASE+4,0x00); //00000000b
    inp(AIO_BASE+5);
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void A_D_Init(void)
 {
    aio_status = AIO_OK;
 	outp(AIO_BASE+11,0x0d);    //00001101b
 }
//
//
//
 unsigned int Read_Thermouple()
 {
 unsigned long input;
 unsigned int input_value;

	input = Read_A_D(0);
    if(input == INVALID) {
        aio_status = AIO_ERR;
        input = 0L;
     }
	input_value = (unsigned int)(input);
	return(input_value);
 }
//
//
//
 unsigned int Read_Alternator_Prior_to_Lamp()
 {
 unsigned long input;
 unsigned int input_value;

    input = Read_A_D(1);
    if(input == INVALID) {
        aio_status = AIO_ERR;
        input = 0L;
        }
	input_value = (unsigned int)(input);
	return(input_value);
 }
//
//
//
 unsigned int Read_Pump()
 {
 unsigned long input;
 unsigned int input_value;

    input = Read_A_D(2);
    if(input == INVALID) {
        aio_status = AIO_ERR;
        input = 0L;
        }
	input_value = (unsigned int)(input);
	return(input_value);
}
//
//
//
 unsigned int Read_Battery_Voltage()
 {
 unsigned long input;
 unsigned int input_value;

    input = Read_A_D(3);
    if(input == INVALID) {
        aio_status = AIO_ERR;
        input = 0L;
        }
	input_value = (unsigned int)(input);
	return(input_value);
 }
//
//
//
 unsigned int Read_Accelerometer()
 {
 unsigned long input;
 unsigned int input_value;

    input = Read_A_D(4);
    if(input == INVALID) {
        aio_status = AIO_ERR;
        input = 0L;
        }
	input_value = (unsigned int)(input);
	return(input_value);
 }
//
//
//
 unsigned int Read_Modem_Signal_Strenght()
 {
 unsigned long input;
 unsigned int input_value;

    input = Read_A_D(5);
    if(input == INVALID) {
        aio_status = AIO_ERR;
        input = 0L;
        }
	input_value = (unsigned int)(input);
	return(input_value);
 }
//
//
//
 unsigned int Read_Radar_Altitude()
 {
 unsigned long input;
 unsigned int input_value;

    input = Read_A_D(6);
    if(input == INVALID) {
        aio_status = AIO_ERR;
        input = 0L;
        }
	input_value = (unsigned int)(input);
	return(input_value);
 }
//
//
//
 unsigned int Read_Alternator_Voltage()
 {
 unsigned long input;
 unsigned int input_value;

    input = Read_A_D(7);
    if(input == INVALID) {
        aio_status = AIO_ERR;
        input = 0L;
        }
	input_value = (unsigned int)(input);
	return(input_value);
 }
//
//
//
 unsigned long Read_A_D(unsigned char channel)
 {
 unsigned long input;                      /* local variable for input data */
 unsigned long time_out;                   /* time out counter */

	// Write A/D Channel to MUX Control
    outp(AIO_BASE+2,channel);
    outp(AIO_BASE+3,channel);
	// Start Conversion
    outp(AIO_BASE,0);
	
	time_out = 0;                   /* set time out */

    while( (inp(AIO_BASE+8) & 0x80) ) {
		time_out++; 		/* increment time out */
		if(time_out>TIME_OUT) {
			return(INVALID);
			}						/* conversion time out */
		}

 #if !defined(DATA_16)

        input = ( inpw(AIO_BASE) ) & 0x0000ffff;
					/* input and mask high nibble */
 #else

        input = inp(AIO_BASE) | (inp(AIO_BASE+1)<<8) & 0x0000ffff;
                        
 #endif

	return(input);                  /* return value to caller */
			
 }
