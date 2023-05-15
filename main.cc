#include "volleyball.h"
#include "battleship.h"
#include <curses.h>

// I don't have much to say, 
// but this is the best implmentation I have seen so far
// It just needs to be finished, thats basically 
// it
// - Alex~


int main() {
	initscr();
	clear();
	noecho();
	cbreak();    
    curs_set(0); // hides blinking caret
    use_default_colors();
    start_color();
    init_pair(WATER, COLOR_BLUE, COLOR_BLUE);
    init_pair(SHIP, COLOR_WHITE, COLOR_BLACK);
    init_pair(SUNK_SHIP, COLOR_BLACK, COLOR_BLUE);
    init_pair(PLACE_SHIP_OK, COLOR_BLACK, COLOR_GREEN);
    init_pair(PLACE_SHIP_BAD, COLOR_BLACK, COLOR_RED);
    init_pair(NORMAL, COLOR_WHITE, -1);
    // Your probably gonna remove this when you do networking..
    printw("use wasd to move, arrow to change direction, enter to place"
           ". Press any key to confirm and to continue"); // placeholder
    getch();

    std::ifstream file("questions.txt");
    auto volleyvall = Volleyball(file);
    auto battleship = Battleship();
    battleship.placeShips();
    // battleship.printUI(1);
    // refresh();
    // getch();
    // while (true) {
    //     battleship.printUI(1);
    //     refresh();
    //     getch();
    // }
    clear();
    endwin();
}