#include <iostream>
#include "Communication.h"

void drawBoard(const std::string& boardState) {
    std::cout << "-------------\n";
    for (int i = 0; i < 3; i++) {
        std::cout << "| ";
        for (int j = 0; j < 3; j++) {
            char cell = boardState[i * 3 + j];
            if (cell == 'X' || cell == 'O') {
                std::cout << cell << " | ";
            } else {
                // Якщо значення не 'X' або 'O', виводимо пусту клітинку
                std::cout << " " << " | ";
            }
        }
        std::cout << "\n-------------\n";
    }
}

int main() {
    try {
        SerialCommunication serial;
        std::string port = "COM5"; // Змініть на актуальний COM порт
        int baudRate = 9600;

        if (!serial.connect(port, baudRate)) {
            std::cerr << "Unable to connect to Arduino!" << std::endl;
            return 1;
        }

        std::cout << "Welcome to the game of Tic-Tac-Toe!" << std::endl;
        std::string response = serial.sendMessage("StartGame\n");

        if (response.find("GameStarted") != std::string::npos) {
            std::cout << "The game has successfully started." << std::endl;
        } else {
            std::cerr << "Unable to start the game on the server." << std::endl;
            serial.disconnect();
            return 1;
        }

        while (true) {
            std::string input;
            std::cout << "Enter your move (1-9) or 'exit' to exit: ";
            std::getline(std::cin, input);

            if (input == "exit") {
                break;
            }

            try {
                int move = std::stoi(input);
                if (move < 1 || move > 9) {
                    std::cout << "Incorrect entry. Enter a number between 1 and 9." << std::endl;
                    continue;
                }

                response = serial.sendMessage("Move " + input + "\n");
                std::cout << "Server response: " << response << std::endl;

                if (response.find("BoardState:") == 0) {
                    std::string boardState = response.substr(12);
                    drawBoard(boardState);
                }

                if (response.find("Wins") != std::string::npos || response == "Draw") {
                    std::cout << "The game is over!" << std::endl;
                    break;
                }
            } catch (...) {
                std::cout << "Input recognition error. Enter the correct number." << std::endl;
            }
        }

        serial.disconnect();
    } catch (const std::exception& e) {
        std::cerr << "An error has occurred: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error!" << std::endl;
    }

    return 0;
}
