1 'Example of DEF SEF statement
10 DEF SEG &6000
20 POKE &0,&55:'Write a byte to memory location &6000:0000
30 PRINT HEX$(PEEK(&0)):'Returns a byte from memory location &6000:0000
