1 'Example of BCC function
10 BLOAD "TEST.BIN",&0,&6000:'Load a binary file to &6000:0000
20 PRINT HEX$(BCC(&0,128,&6000)):'Print BCC of memory of length 128 bytes at &6000:0000
