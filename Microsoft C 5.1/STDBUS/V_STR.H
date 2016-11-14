// file: v_str.h
//
// written by denton marlowe
// (c)1995 hayes targets
//

 #define    _PRINT_APPLICATION_NAME()   com_puts("\r\nTLX DIGITIAL AUTOPILOT\r\n")
 #define    _PRINT_AUTHOR_NAME()        com_puts("written by Denton Marlowe\r\n")
 #define    _PRINT_HAYES_COPYRIGHT()    com_puts("(c)1995 HAYES TARGETS\r\n")

 #ifdef _EPROM
    #pragma message("TLX-EPROM")
 #define    _PRINT_VERSION_STRING() com_puts("\r\nTLX-EPROM\r\n")
 #else
    #pragma message("TLX-DIAGNOSTIC-NOT FOR FLIGHT")
 #define    _PRINT_VERSION_STRING() com_puts("\r\nTLX-DIAGNOSTIC-NOT FOR FLIGHT\r\n")
 #endif
