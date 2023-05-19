#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include <vector>
#include <iostream>
#include <numeric>
#include <set>
#include <cmath>
#include "ncurses.h"

#define rowSize 10
#define colSize 10

#define COLOR_WATER 1
#define COLOR_SHIP 2
#define COLOR_HIT 3
#define COLOR_MISS 4
#define COLOR_PLACE_SHIP_OK 5
#define COLOR_PLACE_SHIP_BAD 6
#define COLOR_CURSOR 7
#define COLOR_NORMAL 8
#define COLOR_PLAYER 9

#define PLAYER1 8
#define PLAYER2 9

enum Key {
    ENTER =  10,
    UP    =  65,
    RIGHT =  67,
    DOWN  =  66,
    LEFT  =  68,
    W     =  119,
    A     =  97,
    S     =  115,
    D     =  100

};

enum gridMap {
    MAP_SHIP = '#',
    MAP_MISS = 'X',
    MAP_HIT = 'O',
    MAP_WATER = ' ',
    MAP_CURSOR = '*'
};

class Battleship {
    std::vector<std::vector<gridMap>> grid;
    int player;
    int shipSizes[5] = {2, 3, 3, 4, 5};
    int currentShip = 0;
    bool isPlacementOver() const;
    int pieces = std::accumulate(std::begin(shipSizes), std::end(shipSizes), 0);
    std::pair<int, int> cursorPostion = {1, 1};
    int direction = RIGHT;
    int attacks = 3;
    bool isPlaceable() const;
public:
    Battleship(int);
    void printUI(int) const;
    bool handleInput(char setInput = '\0');
    void handleMovement(Key);
    void handleDirection(Key);
    void attack();
    bool inPlacementRange(int x, int y) const;
    bool isGameOver() const;
    int getWinner() const;
    void incrementShip();
    void setDirection(Key);
    void setPosition(int, int);
    std::pair<int, int> getPosition() const;
    void setTile(char, int, int);
    char getTile(int, int) const;
    void deincrementPiece();
};

#endif // BATTLESHIP_H


