#include "Communication.h"
#include <iostream>

bool SerialCommunication::connect(const std::string& portName, int baudRate) {
    hSerial = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open port: " << portName << std::endl;
        return false;
    }

    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    dcb.BaudRate = baudRate;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcb)) {
        std::cerr << "Failed to configure port settings." << std::endl;
        CloseHandle(hSerial);
        return false;
    }

    // Налаштування таймаутів для серійного порту
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Failed to set communication timeouts." << std::endl;
        CloseHandle(hSerial);
        return false;
    }

    return true;
}

void SerialCommunication::disconnect() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

std::string SerialCommunication::sendMessage(const std::string& message) {
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Serial port not open." << std::endl;
        return "";
    }

    DWORD bytesWritten;
    if (!WriteFile(hSerial, message.c_str(), message.size(), &bytesWritten, nullptr)) {
        std::cerr << "Failed to write to serial port." << std::endl;
        return "";
    }

    char buffer[256];
    DWORD bytesRead;
    if (!ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
        std::cerr << "Failed to read from serial port." << std::endl;
        return "";
    }
    buffer[bytesRead] = '\0';

    return std::string(buffer);
}
