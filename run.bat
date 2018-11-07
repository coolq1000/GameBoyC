@ECHO off
REM Compile,
tcc src/*.c -o gameboy.exe -DCPU_DEBUG

REM Run,
if %ERRORLEVEL% EQU 0 call gameboy.exe
