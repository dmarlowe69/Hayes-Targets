@echo off
copy /b i17xbios.bin+ptxbios.bin+biosext.img+rs485ext.bin octagon.tmp
lengthen octagon.tmp /l8000h
rem hkdebug copy /b octagon.tmp+atsysbio.bin octagon.bin
copy /b octagon.tmp+octagon.tmp+atsysbio.bin octagon.bin
del octagon.tmp
