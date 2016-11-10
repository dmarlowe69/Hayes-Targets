if exist demo.h goto edit
echo
echo  !!!! ERROR:      Patch aborted because file demo.h not found.
goto EXIT
:edit
echo off
rem	      Enter the name of your editor and patch file on next line.
%1 %2
rem
getsym demo.h labels.s
as1805 /s p
if not errorlevel 1 goto link
echo
echo  !!!! ERROR:      Demo aborted because of assembly error.
goto EXIT
:link
echo.
aalink /s p
if not errorlevel 1 goto EXIT
echo
echo  !!!! ERROR:      Demo aborted because of link error.
:EXIT
