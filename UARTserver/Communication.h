#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class Communication {
public:
    Communication();

    void begin();
    void sendMessage(const String &message);
    String readMessage();
    void drawBoard(char board[3][3]);

private:
    SoftwareSerial mySerial;
};

#endif // COMMUNICATION_H
