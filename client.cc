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
    for (int i = 0; i < 10; i++) {
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
    for (int i = 0; i < 10; i++) {
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

        bool turnOver = false; // place intial ships
        while (!turnOver) {
            turnOver = battleship_client.handleInput();
            printscr(battleship_server, battleship_client);
        }

        askForGrid(battleship_server, stream);
        sendGrid(battleship_client, stream);
        battleship_server.setPlacementOver();

        std::pair<char, char> serverCoord; // match up cursor after placement
        stream >> serverCoord.first;
        stream >> serverCoord.second;
        battleship_server.setPosition(serverCoord.first - '0', serverCoord.second - '0');
        std::pair<int, int> clientCoord = battleship_client.getPosition();
        stream << char(clientCoord.first + '0');  
        stream << char(clientCoord.second + '0');

        char randChar; // get same randomness on client / server
        stream >> randChar;
        srand(int(randChar)); 

        int volleyballTurn = 1;
            while (!battleship_client.isGameOver() && !battleship_server.isGameOver()) {
                double timeLimit = 10;
                bool isCorrect = true;
                while (timeLimit > 0 && isCorrect) { // ask questions back and forth
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
                }
                int shots = 3;
                if (volleyballTurn == client) { // means client won
                    while (shots > 0) {
                        int input = -1;
                        printscr(battleship_server, battleship_client);
                        mvaddstr(16, 0, "You Are Attacking. Choose a tile.");
                        refresh();
                        while (input != LEFT && input != RIGHT && input != DOWN && input != UP && 
                                    input != ENTER && input != W && input != S && input != A && input != D) {
                            input = getch();
                        }
                        if (battleship_server.handleInput(char(input))) {
                            shots--;
                        }
                        if (input == ENTER) {
                            input = int(null);
                        }
                        stream << char(input); 
                    }
                } else {
                    char response;
                    while (shots > 0) {
                        printscr(battleship_server, battleship_client);
                        mvaddstr(16, 0, "You Are being attacked!!");
                        refresh();
                        stream >> response;
                        if (response == null) {
                            response = char(ENTER);
                        }
                        if (battleship_client.handleInput(response)) {
                            shots--;
                        }
                    }
                }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printscr(battleship_server, battleship_client);
        char winnerInfo[200];
        int winner = battleship_server.isGameOver() ? 2 : 1;
        sprintf(winnerInfo,
            "Player %d has sunk all of the ships! They win!!!", winner
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

