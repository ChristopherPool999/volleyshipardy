#include "volleyball.h"
#include "battleship.h"
#include <curses.h>
#include <cstdlib>
#include <string>
using namespace std;
#include <bits/stdc++.h>

int main() {

	try
	{
		//This is code to accept a connection as a server
		boost::asio::io_context io_context;
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 14860); //Change this port to whatever you want
		boost::asio::ip::tcp::acceptor acceptor(io_context, endpoint);
		boost::asio::ip::tcp::iostream stream;
		boost::system::error_code ec;
		std::cout << "Waiting for player (client) to connect..." << std::endl;
		acceptor.accept(stream.socket(), ec);
		std::cout << "Player connected!" << std::endl;
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

            std::ifstream file("questions.txt");
            auto volleyvall = Volleyball(file, stream);
            auto battleship = Battleship(stream);
            battleship.playBattleship();
            erase();
            refresh();

            while (!battleship.isGameOver()) {
                int volleyballTurn = 1;
                double timeLimit = 10;
                while (true) {
                    battleship.printUI();
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
                battleship.printUI();
                battleship.playBattleship();
                if (battleship.isGameOver()) {
                    break;
                }
            }
            erase();
            battleship.printUI();
            char winnerInfo[200];
            sprintf(winnerInfo,
                "Player %d has sunk all of the ships! They win!!!", battleship.getWinner()
            );
            mvaddstr(20, 0, winnerInfo);
            mvaddstr(22, 0, "Press any key to clear terminal and exit");
            getch();
            erase();
            endwin();
            refresh();
            return 0;
		}
		else
			cout << "Sadge.\n";
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}
