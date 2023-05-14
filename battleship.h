#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include <vector>
#include "ncurses.h"
#include <iostream>
#include <numeric>

#define row 10
#define col 10

#define ENTER  10
#define UP     65
#define RIGHT  67
#define DOWN   66
#define LEFT   68
#define W      119
#define A      97
#define S      115
#define D      100

#define ship 'X'

#define WATER 1
#define SHIP 2
#define SUNK_SHIP 3
#define PLACE_SHIP_OK 4
#define PLACE_SHIP_BAD 5
#define NORMAL 6

class Battleship {
    std::vector<std::vector<char>> grid1;
    int shipSizes[5] = {2, 3, 3, 4, 5};
    int currentShip = 0;
    int player1Pieces = std::accumulate(std::begin(shipSizes), std::end(shipSizes), 0);
    int player2Pieces = player1Pieces;
    std::pair<int, int> cursorPostion = {1, 1};
    int direction = RIGHT;
    std::vector<std::pair<int, int>> getValidTiles();
public:
    Battleship();
    void printUI(int) const;
    void placeShips();
    void attack();
    void handleMovement(int);
    void handleDirection(int);
    std::string getTileOutput(int x, int y) const;
};

#endif // BATTLESHIP_H
