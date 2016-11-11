@echo off
rem ***************************************************************************
rem
rem	AMERICAN Batch file
rem
rem ***************************************************************************
:start
rem ***************************************************************************
rem
rem	define prompt as directory name and ">" character
rem	and setup default directory as root and set path
rem
rem ***************************************************************************
prompt $p $g
C:
CD\american\batch
path=c:\;c:\dos;c:\newdos;c:\american\batch;c:\american\1802
rem ***************************************************************************
rem
rem	call menu routine asker.exe
rem
rem	(1)	color of menu text
rem	(2)	color of title
rem	(3)	color of border
rem	(4)	color of function key menu 
rem	(5)	color of prompt message
rem
rem ***************************************************************************
:ask
asker C:\american\batch\american.ask 3 3 3 120 4
rem ***************************************************************************
rem
rem	execute required commands
rem
rem ***************************************************************************
IF ERRORLEVEL 0 if not ERRORLEVEL 1 goto end
IF ERRORLEVEL 1 if not ERRORLEVEL 2 goto debug
IF ERRORLEVEL 2 if not ERRORLEVEL 3 goto eprom
IF ERRORLEVEL 3 if not ERRORLEVEL 4 goto neweprom
IF ERRORLEVEL 4 if not ERRORLEVEL 5 goto easyflow
IF ERRORLEVEL 5 if not ERRORLEVEL 6 goto wp
IF ERRORLEVEL 6 if not ERRORLEVEL 7 goto c
IF ERRORLEVEL 7 if not ERRORLEVEL 8 goto hayes
IF ERRORLEVEL 8 if not ERRORLEVEL 9 goto hayes
IF ERRORLEVEL 9 if not ERRORLEVEL 10 goto end
goto start
rem ***************************************************************************
rem
rem	run debugger
rem
rem ***************************************************************************
:debug
call debugger
goto start
rem ***************************************************************************
rem
rem	run eprom programmer
rem
rem ***************************************************************************
:eprom
call eprom
goto start
rem ***************************************************************************
rem
rem	run new eprom programer
rem
rem ***************************************************************************
:neweprom
call neweprom
goto start
rem ***************************************************************************
rem
rem	run easyflow
rem
rem ***************************************************************************
:easyflow
call mouse
set easyflow=c:\american\easyflow
cd\american\easyflow\document
c:\american\easyflow\easyflow
goto start
rem ***************************************************************************
rem
rem	run wordPrefect
rem
rem ***************************************************************************
:wp
cd\american\wp\document
c:\american\wp\wp
goto start
rem ***************************************************************************
rem
rem	run learn c
rem
rem ***************************************************************************
:c
cd\c\batch
c:\c\batch\c
goto start
rem ***************************************************************************
rem
rem
rem
rem ***************************************************************************
:f7
rem ***************************************************************************
rem
rem	return to hayes menu
rem
rem ***************************************************************************
:hayes
cd\batch
c:\batch\hayes
rem ***************************************************************************
rem
rem	return to dos
rem
rem ***************************************************************************
:end
