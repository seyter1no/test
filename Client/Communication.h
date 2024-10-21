#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <string>
#include <windows.h> // Для використання Windows API

class SerialCommunication {
private:
    HANDLE hSerial = INVALID_HANDLE_VALUE;

public:
    bool connect(const std::string& port, int baudRate);
    std::string sendMessage(const std::string& message);
    void disconnect();
};

#endif // COMMUNICATION_H
