#include "Communication.h"

Communication::Communication() : mySerial(10, 11) {} // RX, TX pins

void Communication::begin() {
    mySerial.begin(9600);
}

void Communication::sendMessage(const String &message) {
    mySerial.println(message);
}

String Communication::readMessage() {
    String message = "";
    while (mySerial.available()) {
        message += (char)mySerial.read();
    }
    return message;
}

void Communication::drawBoard(char board[3][3]) {
    Serial.println("-------------");
    for (int i = 0; i < 3; i++) {
        Serial.print("| ");
        for (int j = 0; j < 3; j++) {
            Serial.print(board[i][j]);
            Serial.print(" | ");
        }
        Serial.println();
        Serial.println("-------------");
    }
}
