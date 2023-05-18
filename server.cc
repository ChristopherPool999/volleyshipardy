#include <string>
#include <boost/asio.hpp>
#include <unistd.h>
#include <iostream>

using namespace std;
using boost::asio::ip::tcp;

#include "volleyball.h"
#include "battleship.h"
#include <curses.h>

void printscr(Battleship &player1, Battleship &player2) {
        erase();
        player1.printUI();
        player2.printUI();
        refresh();
}

int main() {
	try {
		//This is code to accept a connection as a server
		boost::asio::io_context io_context;
		tcp::endpoint endpoint(tcp::v4(), 2101); //Change this port to whatever you want
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
            init_pair(COLOR_CURSOR, COLOR_WHITE, COLOR_BLACK);
            attrset(COLOR_PAIR(COLOR_NORMAL));

            erase();
            refresh();

            std::ifstream file("questions.txt");
            auto volleyvall = Volleyball(file);
            auto battleship_p1 = Battleship(1);
            auto battleship_p2 = Battleship(2);
            printscr(battleship_p1, battleship_p2);

            bool turnOver = false;
            while (!turnOver) {
                turnOver = battleship_p1.handleInput();
                printscr(battleship_p1, battleship_p2);
            }
            turnOver = false;
            while (!turnOver) {
                turnOver = battleship_p2.handleInput();
                printscr(battleship_p1, battleship_p2);
            }

            // until here is good
            while (!battleship_p1.isGameOver()) {
                int volleyballTurn = 1;
                double timeLimit = 10;
                while (true) {
                    battleship_p1.printUI();
                    std::pair<bool, double> results = volleyvall.playGame(volleyballTurn, timeLimit);
                    timeLimit = results.second;
                    if (!results.first) {
                        timeLimit = 10;
                        break;
                    }
                    volleyballTurn = (volleyballTurn == 1 ? 2 : 1);
                    erase();
                }
                erase();
                battleship_p1.printUI();
                battleship_p1.handleInput();
                if (battleship_p1.isGameOver()) {
                    break;
                }
            }
            erase();
            battleship_p1.printUI();
            char winnerInfo[200];
            sprintf(winnerInfo,
                "Player %d has sunk all of the ships! They win!!!", battleship_p1.getWinner()
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
