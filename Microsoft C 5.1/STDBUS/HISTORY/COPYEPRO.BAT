@echo off
if "%1"=="" goto usage
COPY EPROM.BAT %1
COPY DBEPROM.BUG %1
COPY 27XXX.CFG %1
COPY TYPE.CFG %1
COPY EPROM.CMD %1
COPY DBEPROM.EXE %1
COPY EPROM_27.EXE %1
COPY DBEPROM.HLP %1
goto dos
:usage
echo error - must specify a destination
echo example:
echo copyepro a:
:dos
