#include <Arduino.h>

char board[3][3] = { {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'} };
bool isGameStarted = false;
int lastServerMove = -1; // Зберігаємо останній хід комп'ютера

void setup() {
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');

        if (command == "StartGame") {
            resetBoard();
            isGameStarted = true;
            Serial.println("GameStarted");
            printBoardGraphically();
        } else if (command.startsWith("Move") && isGameStarted) {
            int position = command.substring(5).toInt();
            int row = (position - 1) / 3;
            int col = (position - 1) % 3;
            if (position >= 1 && position <= 9 && board[row][col] != 'X' && board[row][col] != 'O') {
                board[row][col] = 'X'; // Гравець ставить X
                Serial.println("PlayerMove: " + String(position)); // Виводимо хід гравця
                printBoardGraphically(); // Виводимо дошку після ходу гравця

                if (!checkGameStatus()) { // Перевіряємо статус гри після ходу гравця
                    serverMove(); // Хід сервера
                    Serial.println("ServerMove: " + String(lastServerMove)); // Виводимо хід сервера
                    printBoardGraphically(); // Виводимо дошку після ходу сервера
                    checkGameStatus(); // Перевіряємо статус гри після ходу сервера
                }
            } else {
                Serial.println("InvalidMove");
            }
        }
    }
}

void resetBoard() {
    char initialBoard[3][3] = { {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'} };
    memcpy(board, initialBoard, sizeof(initialBoard));
    lastServerMove = -1; // Скидаємо останній хід комп'ютера
}

void serverMove() {
    // Логіка для комп'ютера, щоб взяти наступну доступну позицію
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                board[i][j] = 'O'; // Хід сервера
                lastServerMove = i * 3 + j + 1; // Зберігаємо позицію останнього ходу
                return; // Вихід після здійснення ходу
            }
        }
    }
}

bool checkGameStatus() {
    if (checkWin('X')) {
        Serial.println("X Wins");
        isGameStarted = false; // Завершуємо гру
        return true;
    } else if (checkWin('O')) {
        Serial.println("O Wins");
        isGameStarted = false; // Завершуємо гру
        return true;
    } else if (isBoardFull()) {
        Serial.println("Draw");
        isGameStarted = false; // Завершуємо гру
        return true;
    }
    return false;
}

bool checkWin(char player) {
    // Перевірка рядків, стовпців та діагоналей
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}

bool isBoardFull() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return false;
            }
        }
    }
    return true;
}

void printBoardGraphically() {
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
    Serial.println(); // Додати порожній рядок після виводу дошки
}