1 'Example of FPEEK function and FPOKE statement
10 FPOKE &0,1.234,&6000:'Write a floating point number at 6000:0000
20 PRINT FPEEK(&0,&6000):'Treat the 1st 4 bytes at &6000:0000 as floating point number and print it
