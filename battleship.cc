#include "battleship.h"

Battleship::Battleship() {
    grid1 = std::vector<std::vector<char>> (row + 1, std::vector<char>(col + 1, ' ')); 
}

std::string Battleship::getTileOutput(int tileX, int tileY) const {
    int length = shipSizes[currentShip] - 1;
    int cursorX = cursorPostion.first;
    int cursorY = cursorPostion.second;

    std::string output = std::string(1, ship); 
    if (direction == RIGHT && cursorY == tileY) {
         if (tileX >= cursorX && tileX <= cursorX + length) {
            return output;
         }
    } else if (direction == LEFT && cursorY == tileY) {
         if (tileX >= cursorX - length && tileX <= cursorX) {
            return output;
         }
    } else if (direction == UP && cursorX == tileX) {
         if (tileY >= cursorY - length && tileY <= cursorY) {
            return output;
         }   
    } else if (direction == DOWN && cursorX == tileX) {
         if (tileY >= cursorY && tileY <= cursorY + length ) {
            return output;
         }  
    }
    std::string text = "";
    text.push_back(grid1[tileY][tileX]);
    return text;
}

void Battleship::printUI(int player) const {
    std::string left = (player == 1 ? "   YOU   " : "PLAYER 1");
    std::string right = (player == 2 ? "   YOU   " : "PLAYER 2");
    printw("    %s           %d        %s            \n", left.c_str(), 60, right.c_str());
    printw("___________________________________________________________________________\n");
    printw("  0123456789     |       0123456789  \n"  );

    char l = 'A';
    for (int y = 0; y < row; y++) {
        std::string text = "";
        text = text + l + ' ';
        for (int x = 0; x < col; x++) {
            text += getTileOutput(x, y);
        }
        text += "     |     ";
        text = text + l++ + ' ';
        for (int x = 0; x < col; x++) {
            text += getTileOutput(x, y);
        }
        text = (text += '\n').c_str();
        for (int i = 0; i < int(text.size()); i++) {
            if (text[i] == ' ' && ((i > 1 && i <= col + 1) || (i > int(text.size()) - col - 2))) {
                attrset(COLOR_PAIR(WATER));
            } else if (text[i] == ship) {
                attrset(COLOR_PAIR(SHIP));
            } else {
                attrset(COLOR_PAIR(NORMAL));
            }
            printw("%c", text[i]);
        }
    }
}

void Battleship::attack() {
    ;
}

void Battleship::handleMovement(int KEY) {
    if (KEY == A && cursorPostion.first > 0 
            && (direction != LEFT || cursorPostion.first + 1 > shipSizes[currentShip])) {
        cursorPostion.first--;
    } else if (KEY == D && cursorPostion.first < col - 1
            && (direction != RIGHT || cursorPostion.first + shipSizes[currentShip] < col)) {
         cursorPostion.first++;
    } else if (KEY == S && cursorPostion.second < row - 1
            && (direction != DOWN || cursorPostion.second + shipSizes[currentShip] < row)) {
        cursorPostion.second++;
    } else if (KEY == W && cursorPostion.second > 0
            && (direction != UP || cursorPostion.second + 1 > shipSizes[currentShip])) {
        cursorPostion.second--;
    }
}

void Battleship::handleDirection(int KEY) {
    if (KEY == RIGHT && cursorPostion.first < row + 1 - shipSizes[currentShip]) {
        direction = RIGHT;
    } else if (KEY == LEFT && cursorPostion.first + 2 > shipSizes[currentShip]) {
        direction = LEFT;
    } else if (KEY == DOWN && cursorPostion.second < col + 1 - shipSizes[currentShip]) {
        direction = DOWN;
    } else if (KEY == UP && cursorPostion.second + 2 > shipSizes[currentShip]) {
        direction = UP;
    }
}

void Battleship::getPlacementTiles() {
    placement.first = std::set<std::string> {};
    placement.second = true;
    for (int i = 0; i < shipSizes[currentShip]; i++) {
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
        if (y > col || x > row || y < 0 || x < 0) {
            placement.second = false;
            return;
        }
        if (grid1[y][x] == ship) {
            placement.second = false;
        }
        // std::string key = x + " " + y; // uncommenting this 
        // placement.first.insert(key); // gives segfault
    }
}

void Battleship::placeShips() {
    while (currentShip < int(sizeof(shipSizes) / sizeof(int))) {
        erase();
        printUI(1);
        refresh();
        int KEY = getch();
        if (KEY == W || KEY == A || KEY == S || KEY == D) {
            handleMovement(KEY);
        } else if (KEY == LEFT || KEY == RIGHT || KEY == UP || KEY == DOWN) {
            handleDirection(KEY);
        } else if (KEY == ENTER && placement.second) {
            for (int i = 0; i < shipSizes[currentShip]; i++) {
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
                grid1[y][x] = ship;
            }
            currentShip++;            
        }
        getPlacementTiles();
    }
}