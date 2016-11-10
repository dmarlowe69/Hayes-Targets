echo off
:start
if %1t == t goto usage
if %2t == t goto usage
if not exist %2.err goto prep
erase %2.err
:prep
if not exist %2.m goto asm
asprep %2
if errorlevel 1 goto error
:asm
if not exist %2.s goto nos
as%1 /sx-e %2 -obj\%2
if errorlevel 1 goto error
:done
if exist %2.m del %2.s
goto dos
:error
copy obj\%2.err
del  obj\%2.err
goto dos
:nos
echo .
echo can't find %2.s.
echo .
goto dos
:usage
echo Usage is:
echo .
echo ASM p f
echo .
echo where p is the name of the assembler
echo and f is the name of the program to assemble (no extension)
echo .
:dos
