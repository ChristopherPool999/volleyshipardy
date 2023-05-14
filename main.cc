#include "volleyball.h"
#include "battleship.h"
#include <curses.h>

int main() {
	initscr();
	clear();
	noecho();
	cbreak();    
    curs_set(0); // hides blinking caret
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