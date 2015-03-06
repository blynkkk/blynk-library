@echo off

REM === Edit these lines to match your need ===
set COMM_PORT=COM1
set COMM_BAUD=9600
set SERV_ADDR=blynk-test-east.cloudapp.net
set SERV_PORT=8442

REM === Edit lines below only if absolutely sure what you're doing ===
echo Connecting device at %COMM_PORT% to %SERV_ADDR%:%SERV_PORT%...
:restart
  com2tcp --baud %COMM_BAUD% --ignore-dsr \\.\%COMM_PORT% %SERV_ADDR% %SERV_PORT%
  sleep 2
  echo Reconnecting...
goto restart

