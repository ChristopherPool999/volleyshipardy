//Some of this code is from Boost's documentation
#include "volleyball.h"
#include "battleship.h"
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;
#include <curses.h>

int main()
{
	try
	{
		boost::asio::ip::tcp::iostream stream("localhost", "14860"); //[1] == host, [2] == port
		if (!stream) //Connection failed
		{
			cout << "Unable to connect: " << stream.error().message() << endl;
			return EXIT_FAILURE;
		}

		while (true) {
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
	}
	catch (exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}
}

