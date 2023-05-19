#include <string>
#include <boost/asio.hpp>
#include <unistd.h>
#include <iostream>

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

void sendGrid(Battleship &serverGrid, tcp::iostream& stream) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            char ins = serverGrid.getTile(i, j);
            if (ins == ' ') {
                ins = null;
            } 
            stream << ins;
        }
    }  
}

void askForGrid(Battleship &clientGrid, tcp::iostream& stream) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            char temp;
            stream >> temp;
            if (temp == null) {
                temp = ' ';
            } 
            clientGrid.setTile(static_cast<gridMap>(temp), i, j);
        }
    }
}

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

void printscr(Battleship &player1, Battleship &player2) {
        erase();
        player1.printUI(server);
        player2.printUI(server);
        refresh();
}

int main() {
	try {
		//This is code to accept a connection as a server
		boost::asio::io_context io_context;
		tcp::endpoint endpoint(tcp::v4(), 48342); //Change this port to whatever you want
		tcp::acceptor acceptor(io_context, endpoint);
		tcp::iostream stream;
		boost::system::error_code ec;
		cout << "Waiting for a Client to connect...\n";
		acceptor.accept(stream.socket(), ec);
		cout << "Client connected!\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));

		if (!ec) {
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

            erase();
            refresh();

            std::ifstream file("questions.txt");
            auto volleyball = Volleyball(file, server);
            auto battleship_client = Battleship(2);
            auto battleship_server = Battleship(1);
            printscr(battleship_server, battleship_client);

            bool turnOver = false; // place intial ships
            while (!turnOver) {
                turnOver = battleship_server.handleInput();
                printscr(battleship_server, battleship_client);
            }

            sendGrid(battleship_server, stream);
            askForGrid(battleship_client, stream);
            battleship_client.setPlacementOver();

            std::pair<int, int> serverCoord = battleship_server.getPosition(); // match up cursor after placement
            stream << char(serverCoord.first + '0');  
            stream << char(serverCoord.second + '0');
            std::pair<char, char> clientCoord;
            stream >> clientCoord.first;
            stream >> clientCoord.second;
            battleship_client.setPosition(clientCoord.first - '0', clientCoord.second - '0');
            
            srand(time(0)); // get same randomness on client / server
            char randSeed = 'A' + rand() % 25;
            srand(int(randSeed));
            stream << randSeed; 

            int volleyballTurn = 1;
            while (!battleship_client.isGameOver() && !battleship_server.isGameOver()) {
                double timeLimit = 10;
                bool isCorrect = true;
                while (timeLimit > 0 && isCorrect) { // ask questions back and forth
                    printscr(battleship_server, battleship_client);
                    std::pair<bool, double> results;
                    results = volleyball.playGame(timeLimit, volleyballTurn == server);
                    if (volleyballTurn == server) {
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
                if (volleyballTurn == server) { // means server won
                    while (shots > 0) {
                        int input = -1;
                        printscr(battleship_server, battleship_client);
                        mvaddstr(16, 0, "You Are Attacking. Choose a tile.");
                        refresh();
                        while (input != LEFT && input != RIGHT && input != DOWN && input != UP && 
                                    input != ENTER && input != W && input != S && input != A && input != D) {
                            input = getch();
                        }
                        if (battleship_client.handleInput(char(input))) {
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
                        if (battleship_server.handleInput(response)) {
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
                "Player %d has sunk all of the ships! !!!", winner
            );
            mvaddstr(20, 0, winnerInfo);
            mvaddstr(22, 0, "Press any key to clear terminal and exit");
            getch();
            erase();
            endwin();
            refresh();
		}
		else
			cout << "Sadge.\n";
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}
    return 0;
}

