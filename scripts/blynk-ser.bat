@echo off

REM === Edit these lines to match your need ===
set COMM_PORT=COM1
set COMM_BAUD=115200
set SERV_ADDR=blynk.lan.ua
set SERV_PORT=8181

REM === Edit lines below only if absolutely sure what you're doing ===
:restart
  com2tcp --baud %COMM_BAUD% --ignore-dsr \\.\%COMM_PORT% %SERV_ADDR% %SERV_PORT%
  sleep 2
goto restart
