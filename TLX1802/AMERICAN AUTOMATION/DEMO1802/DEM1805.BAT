as1805 /s-ex demo
echo off
if not errorlevel 1 goto link
echo
echo  !!!! ERROR:      Demo aborted because of assembly error.
goto EXIT
:link
aalink /s @demo -demo
if not errorlevel 1 goto debug
echo
echo  !!!! ERROR:      Demo aborted because of link error.
goto EXIT
:debug
db1805 com1 demo  %1
:EXIT
