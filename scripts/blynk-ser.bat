@echo off
setlocal EnableDelayedExpansion

REM === Edit these lines to match your need ===

set COMM_PORT=COM1
set COMM_BAUD=9600
set SERV_ADDR=blynk.cloud
set SERV_PORT=80

REM === Edit lines below only if absolutely sure what you're doing ===

rem Get command line options
set SCRIPTS_PATH=%~dp0

:loop
IF NOT "%1"=="" (
    IF "%1"=="-c" set COMM_PORT=%2& SHIFT & SHIFT & GOTO :loop
    IF "%1"=="-b" set COMM_BAUD=%2& SHIFT & SHIFT & GOTO :loop
    IF "%1"=="-s" set SERV_ADDR=%2& SHIFT & SHIFT & GOTO :loop
    IF "%1"=="-p" set SERV_PORT=%2& SHIFT & SHIFT & GOTO :loop

    CALL :usage
    GOTO :eof
)

rem Find ports
set PORTS=
for /f "tokens=4 delims=: " %%A in ('mode^|findstr "COM[0-9]*:"') do IF not [%%A] == [] set PORTS=!PORTS! %%A
set PORTS=!PORTS:~1!

rem Check port
rem Skip check if no ports at all - Windows bug?
if not "x%PORTS%"=="x~1" (
    if "x!PORTS:%COMM_PORT%=!"=="x%PORTS%" (
        echo %COMM_PORT% not found, or may be busy.
        set /p COMM_PORT="Select serial port [ %PORTS% ]: "
    )
)

rem Create exe
if not exist "%SCRIPTS_PATH%\com2tcp.exe" (
    copy "%SCRIPTS_PATH%\com2tcp.bin" "%SCRIPTS_PATH%\com2tcp.exe" > NUL
)

rem Do the job
echo Connecting device at %COMM_PORT% to %SERV_ADDR%:%SERV_PORT%...

rem Try resetting board
rem mode %COMM_PORT%:%COMM_BAUD%,N,8,1 >nul

:restart
  "%SCRIPTS_PATH%\com2tcp.exe" --baud %COMM_BAUD% --ignore-dsr \\.\%COMM_PORT% %SERV_ADDR% %SERV_PORT%
  echo Reconnecting in 3s...
  timeout /T 3
goto restart


goto:eof

:usage
    echo.
    echo.   This script redirects serial communication to the server.
    echo.
    echo.   You can specify port, baud rate, and server endpoint like this:
    echo.     blynk-ser.bat -c ^<serial port^> -b ^<baud^> -s ^<server^> -p ^<port^>
    echo.
    echo.   The defaults are:
    echo.     -c    /dev/ttyUSB0       (on Linux)
    echo.           COM1               (on Windows)
    echo.           /dev/tty.usbserial (on OSX)
    echo.     -b    9600
    echo.     -s    blynk.cloud
    echo.     -p    80
    echo.
    echo.   If the specified serial port is not found, it will ask to enter another one.
    echo.   The script also tries to reestablish connection if it was lost.
goto:eof
