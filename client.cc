//Some of this code is from Boost's documentation
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;
#include "volleyball.h"
#include "battleship.h"
#include <curses.h>
#include <random>
#include <thread>

#define server 1
#define client 2

#define null 'n'

void debug_showInputs(Battleship& _server, Battleship& _client) {
    erase();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            mvaddch(i, j, _server.getTile(i, j));
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            mvaddch(i, j + 15, _client.getTile(i, j));
        }
    }
    refresh();
    exit(1);
}

void sendGrid(Battleship &clientGrid, tcp::iostream& stream) {
    for (int i = 0; i < 10; i++) { // tells server our grid
        for (int j = 0; j < 10; j++) {
            char ins = clientGrid.getTile(i, j);
            if (ins == ' ') {
                ins = null;
            } 
            stream << ins;
        }
    }  
}

void askForGrid(Battleship &serverGrid, tcp::iostream& stream) {
    for (int i = 0; i < 10; i++) { // reads the servers grid
        for (int j = 0; j < 10; j++) {
            char temp;
            stream >> temp;
            if (temp == null) {
                temp = ' ';
            } 
            serverGrid.setTile(static_cast<gridMap>(temp), i, j);
        }
    }
}

void printscr(Battleship &player1, Battleship &player2) {
        erase();
        player1.printUI(client);
        player2.printUI(client);
        refresh();
}

int main() {
	try {
		tcp::iostream stream("localhost", "48342"); //[1] == host, [2] == port
		if (!stream) {
			cout << "Unable to connect: " << stream.error().message() << endl;
			return EXIT_FAILURE;
		}

        initscr();
        clear();
        noecho();
        cbreak();    
        curs_set(0); // hides blinking caret
        use_default_colors();
        start_color();
        init_pair(COLOR_WATER, COLOR_BLUE, COLOR_BLUE);
        init_pair(COLOR_SHIP, COLOR_WHITE, COLOR_WHITE);
        init_pair(COLOR_HIT, COLOR_BLACK, COLOR_GREEN);
        init_pair(COLOR_MISS, COLOR_BLACK, COLOR_RED);
        init_pair(COLOR_PLACE_SHIP_BAD, COLOR_BLACK, COLOR_RED);
        init_pair(COLOR_PLACE_SHIP_OK, COLOR_BLACK, COLOR_GREEN);
        init_pair(COLOR_NORMAL, COLOR_WHITE, -1);
        init_pair(COLOR_CURSOR, COLOR_BLACK, COLOR_BLACK);
        init_pair(COLOR_PLAYER, COLOR_YELLOW, COLOR_BLACK);
        attrset(COLOR_PAIR(COLOR_NORMAL));

        std::ifstream file("questions.txt");
        auto volleyball = Volleyball(file, client);
        auto battleship_client = Battleship(client);
        auto battleship_server = Battleship(server);
        printscr(battleship_server, battleship_client);

        bool turnOver = false;
        while (!turnOver) {
            turnOver = battleship_client.handleInput();
            printscr(battleship_server, battleship_client);
        }

        askForGrid(battleship_server, stream);
        sendGrid(battleship_client, stream);
        battleship_server.setPlacementOver();

        char randChar;
        stream >> randChar;
        srand(int(randChar)); // get same randomness on client / server

        int volleyballTurn = 1;
            while (!battleship_client.isGameOver()) {
                double timeLimit = 10;
                bool isCorrect;
                while (true) {
                    printscr(battleship_server, battleship_client);
                    std::pair<bool, double> results;
                    results = volleyball.playGame(timeLimit, volleyballTurn == client);
                    if (volleyballTurn == client) {
                        timeLimit = min(int(ceil(results.second)), 9);
                        timeLimit = max(int(timeLimit), 0);
                        isCorrect = results.first;
                        stream << (isCorrect ? '1' : '0');
                        stream << char(int(timeLimit) + '0');
                    } else {
                        char output;
                        stream >> output;
                        isCorrect = (output == '1');
                        stream >> output; 
                        timeLimit = int(output - '0');
                    }
                    volleyball.showResults(isCorrect, timeLimit);
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    volleyballTurn = (volleyballTurn == 1 ? 2 : 1);
                    if (timeLimit <= 0 || !isCorrect) {
                        break;
                    }       
                }
                if (volleyballTurn == client) { // means client got it wrong and we swapped turns for next round
                    int shots = 3;
                    while (shots) {
                        int input = -1;
                        printscr(battleship_client, battleship_server);
                        mvaddstr(16, 20, "You Are Attacking. Choose a tile.");
                        refresh();
                        while (input != LEFT && input != RIGHT && input != DOWN && input != UP && 
                                    input != ENTER && input != W && input != S && input != A && input != D) {
                            input = getch();
                        }
                        if (battleship_server.handleInput(char(input))) {
                            shots--;
                        }
                        stream << char(input); 
                    }
                } else {
                    char response = null;
                    while (int(response) != ENTER) {
                        stream >> response;
                        battleship_server.handleInput(response);
                        printscr(battleship_server, battleship_client);
                        mvaddstr(16, 20, "You Are being attacked!!");
                        refresh();
                    }
                }
        }
        printscr(battleship_server, battleship_client);
        // battleship_client.printUI();
        char winnerInfo[200];
        sprintf(winnerInfo,
            "Player %d has sunk all of the ships! They win!!!", battleship_client.getWinner()
        );      
        mvaddstr(20, 0, winnerInfo);
        mvaddstr(22, 0, "Press any key to clear terminal and exit");
        getch();
        erase();
        endwin();
        refresh();
	}
	catch (exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}
    return 0;
}

