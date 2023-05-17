#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include <boost/asio.hpp>
#include <vector>
#include <iostream>
#include <numeric>
#include <set>
#include <cmath>
#include "ncurses.h"

#define rowSize 10
#define colSize 10 // 

#define ENTER  10
#define UP     65
#define RIGHT  67
#define DOWN   66
#define LEFT   68
#define W      119
#define A      97
#define S      115
#define D      100

#define COLOR_WATER 1
#define COLOR_SHIP 2
#define COLOR_HIT 3
#define COLOR_MISS 4
#define COLOR_PLACE_SHIP_OK 5
#define COLOR_PLACE_SHIP_BAD 6
#define COLOR_CURSOR 7
#define COLOR_NORMAL 8

#define PLAYER1 8
#define PLAYER2 9

enum gridMap {
    MAP_SHIP = '#',
    MAP_MISS = 'X',
    MAP_HIT = 'O',
    MAP_WATER = ' ',
    MAP_CURSOR = '*'
};

class Battleship {
    std::vector<std::vector<gridMap>> grid1;
    int shipSizes[5] = {2, 3, 3, 4, 5};
    int currentShip = 0;
    bool isPlacementOver() const;
    int player1Pieces = std::accumulate(std::begin(shipSizes), std::end(shipSizes), 0);
    int player2Pieces = player1Pieces;
    std::pair<int, int> cursorPostion = {1, 1};
    int direction = RIGHT;
    int attacks = 3;
    boost::asio::ip::tcp::iostream &stream;
    bool isPlaceable() const;
public:
    Battleship(boost::asio::ip::tcp::iostream&);
    void printUI() const;
    void playBattleship();
    void attack();
    void handleMovement(int);
    void handleDirection(int);
    bool inPlacementRange(int x, int y) const;
    bool isGameOver() const;
    int getWinner() const;
};

#endif // BATTLESHIP_H

