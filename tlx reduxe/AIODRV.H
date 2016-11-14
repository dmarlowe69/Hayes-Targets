//  file: AIODRV.H

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
//      I/O addresses of AIO control registers
//

 #define AIO_BASE 0x0300            // base address of I/O board

//
//      AIO_STATUS Defintions
//
 extern signed int aio_status;

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
