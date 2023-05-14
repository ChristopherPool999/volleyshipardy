#include "volleyball.h"
#include "battleship.h"
#include <curses.h>

int main() {
	initscr();
	clear();
	noecho();
	cbreak();    
    curs_set(0); // hides blinking caret
    use_default_colors();
    start_color();
    init_pair(WATER, COLOR_BLUE, COLOR_BLUE);
    init_pair(SHIP, COLOR_BLACK, COLOR_WHITE);
    init_pair(SUNK_SHIP, COLOR_BLACK, COLOR_BLUE);
    init_pair(PLACE_SHIP_OK, COLOR_BLACK, COLOR_GREEN);
    init_pair(PLACE_SHIP_BAD, COLOR_BLACK, COLOR_RED);
    init_pair(NORMAL, COLOR_WHITE, -1);

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