@echo off

REM Компіляція клієнтського додатку
g++ -o Build\main.exe Client\ClientMain.cpp Client\ClientCommunication.cpp Client\Communication.h

REM Компіляція Arduino програми через платформу Arduino (IDE або arduino-cli)
arduino-cli compile --fqbn arduino:avr:nano UARTserver\UARTserver.ino