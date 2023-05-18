#include "battleship.h"

Battleship::Battleship(int player) : player(player) {
    grid = std::vector<std::vector<gridMap>> (rowSize + 1, std::vector<gridMap>(colSize + 1, MAP_WATER)); 
}

bool Battleship::isPlacementOver() const {
    return currentShip >= int(sizeof(shipSizes) / sizeof(int));
}

bool Battleship::isGameOver() const {
    return pieces == 0 || pieces == 0;
}

int Battleship::getWinner() const {
    return (pieces == 0 ? 2 : 1);
}


void Battleship::incrementShip() {
    if (currentShip >= sizeof(shipSizes) / sizeof(int)) {
        throw std::runtime_error("ship is already at max size.\n");
    } 
    currentShip++;
}


void Battleship::setDirection(Key this_key) {
    if (this_key != UP || this_key != RIGHT || this_key != DOWN || direction != LEFT) {
        throw std::runtime_error("Invalid Key on setDirection function.\n");
    }
        direction = this_key;
}


void Battleship::setPosition(int row, int col) {
    if (row < 0 || col < 0 || row >= rowSize || col >= colSize) {
        throw std::runtime_error("position is out of map.\n");
    }
    cursorPostion = {col, row}; 
}

void Battleship::deincrementPiece() {
    pieces--;
}


void Battleship::setTile(char val, int row, int col) {
    gridMap tile  = static_cast<gridMap>(val);
    grid[row][col] = tile;
}

bool Battleship::inPlacementRange(int col, int row) const {
    int length = !isPlacementOver() ? shipSizes[currentShip] - 1 : 0;
    int cursorX = cursorPostion.first;
    int cursorY = cursorPostion.second;

    return (direction == RIGHT && cursorY == row
            && col >= cursorX && col <= cursorX + length)
            
            || (direction == LEFT && cursorY == row
            && col >= cursorX - length && col <= cursorX)

            || (direction == UP && cursorX == col
            && row >= cursorY - length && row <= cursorY)

            || (direction == DOWN && cursorX == col
            &&row >= cursorY && row <= cursorY + length);
}

void Battleship::attack() {
    if (grid[cursorPostion.second][cursorPostion.first] == MAP_SHIP) {
        pieces--;
        grid[cursorPostion.second][cursorPostion.first] = MAP_HIT;
        attacks--;
    } else if (grid[cursorPostion.second][cursorPostion.first] == MAP_WATER) {
        grid[cursorPostion.second][cursorPostion.first] = MAP_MISS;
        attacks--;
    }
}

void Battleship::handleMovement(Key this_key) {
    int length = !isPlacementOver() ? shipSizes[currentShip] - 1 : 0;
    if (this_key == A && cursorPostion.first > 0 
            && (direction != LEFT || cursorPostion.first > length)) {
        cursorPostion.first--;
    } else if (this_key == D && cursorPostion.first < colSize - 1
            && (direction != RIGHT || cursorPostion.first + length + 1 < colSize)) {
         cursorPostion.first++;
    } else if (this_key == S && cursorPostion.second < rowSize - 1
            && (direction != DOWN || cursorPostion.second + length + 1 < rowSize)) {
        cursorPostion.second++;
    } else if (this_key == W && cursorPostion.second > 0
            && (direction != UP || cursorPostion.second > length)) {
        cursorPostion.second--;
    }
}

void Battleship::handleDirection(Key this_key) {
    int length = !isPlacementOver() ? shipSizes[currentShip] - 1 : 0;
    if (this_key == RIGHT && cursorPostion.first < rowSize - length) {
        direction = RIGHT;
    } else if (this_key == LEFT && cursorPostion.first + 1 > length) {
        direction = LEFT;
    } else if (this_key == DOWN && cursorPostion.second < colSize - length) {
        direction = DOWN;
    } else if (this_key == UP && cursorPostion.second + 1 > length) {
        direction = UP;
    }
}

bool Battleship::isPlaceable() const {
    int length = !isPlacementOver() ? shipSizes[currentShip] - 1 : 0;
    for (int i = 0; i <= length; i++) {
        int x = cursorPostion.first;
        int y = cursorPostion.second;
        if (direction == UP) {
            y -= i;
        } else if (direction == RIGHT) {
            x += i;
        } else if (direction == LEFT) {
            x -= i;
        } else if (direction == DOWN) {
            y += i;
        }
        if (grid[y][x] == MAP_SHIP || y > colSize || x > rowSize || y < 0 || x < 0) {
            return false;
        }
    }
    return true;
}

bool Battleship::handleInput(char setInput) {
    Key this_key = static_cast<Key>(setInput ? setInput : getch());
    if (this_key == W || this_key == A || this_key == S || this_key == D) {
        handleMovement(this_key);
    } else if (this_key == LEFT || this_key == RIGHT || this_key == UP || this_key == DOWN) {
        handleDirection(this_key);
    } else if (this_key == ENTER && isPlaceable() && !isPlacementOver()) {
        for (int i = 0; i < int(shipSizes[currentShip]); i++) {
            int x = cursorPostion.first;
            int y = cursorPostion.second;
            if (direction == UP) {
                y -= i;
            } else if (direction == RIGHT) {
                x += i;
            } else if (direction == LEFT) {
                x -= i;
            } else if (direction == DOWN) {
                y += i;
            }
            grid[y][x] = MAP_SHIP;
        }
        currentShip++; 
        int length = !isPlacementOver() ? shipSizes[currentShip] - 1 : 0; // handling ship growth while near out of bounds
        if (direction == RIGHT && cursorPostion.first >= rowSize - length) {
            cursorPostion.first--;
        } else if (direction == LEFT && cursorPostion.first - length < 0) {
            cursorPostion.first++;
        } else if (direction == DOWN && cursorPostion.second >= colSize - length) {
            cursorPostion.second--;
        } else if (direction == UP && cursorPostion.second - length < 0) {
            cursorPostion.second++;
        }
        if (isPlacementOver()) {
            return true;
        }
    } else if (this_key == ENTER && isPlacementOver()) {
        attack();
        if (attacks == 0) {
            attacks = 3;
            return true;
        }
    }
    return false;
}

#define space_between_boards 11

void Battleship::printUI() const {
    mvaddstr(0, 3, "PLAYER 1");
    mvaddstr(0, 24, "PLAYER 2");  
    mvaddstr(1, 0, "__________________________________");
    mvaddstr(14, 0, "__________________________________");
    for (int i = 2; i < 15; i++) {
        mvaddstr(i, 17, "|");
        mvaddstr(i, 34, "|");
    }
    mvaddstr(2, 17, "|");
    mvaddstr(3, 2, "0123456789");    
    mvaddstr(3, 2 + colSize + (space_between_boards / 2), "|"); 
    mvaddstr(3, 2 + colSize + space_between_boards, "0123456789");       
    char letter = 'A';
    for (int i = 4; i < 14; i++) {
        mvaddch(i, 0, letter);
        mvaddch(i, 33, letter);
        letter++;
    }

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            int playerGrid = (player == 1 ? 2 + j : 2 + j + 21);
            if (grid[i][j] == MAP_SHIP && isPlacementOver()) { // hide ships after placement 
                attrset(COLOR_PAIR(
                    (i == cursorPostion.second && j == cursorPostion.first) ? COLOR_CURSOR : COLOR_WATER)
                );
                mvaddch(4 + i, playerGrid, MAP_WATER);         
                continue;
            }
            if (j == cursorPostion.first && i == cursorPostion.second && isPlacementOver()) {
                attrset(COLOR_PAIR(COLOR_CURSOR)); 
            }
            else if (inPlacementRange(j, i)) { // shows area you would place tiles on
                isPlaceable() ? attrset(COLOR_PAIR(COLOR_PLACE_SHIP_OK)) : attrset(COLOR_PAIR(COLOR_PLACE_SHIP_BAD));
                mvaddch(4 + i, playerGrid, MAP_SHIP);         
                continue;             
            }
            else if (grid[i][j] == MAP_HIT) {
                attrset(COLOR_PAIR(COLOR_HIT));
            } else if (grid[i][j] == MAP_MISS) {
                attrset(COLOR_PAIR(COLOR_MISS));
            } else if (grid[i][j] == MAP_WATER) {
                attrset(COLOR_PAIR(COLOR_WATER));
            } else if (grid[i][j] == MAP_SHIP) {   
                attrset(COLOR_PAIR(COLOR_SHIP));
            }
            mvaddch(4 + i, playerGrid, grid[i][j]);
        }
        attrset(COLOR_PAIR(COLOR_NORMAL));
    }
}
