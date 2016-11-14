tlxprod.h : tlxprod.o
        AALINK /S tlxprod

tlxprod.o : tlxprod.s
        AS1805 /S tlxprod
