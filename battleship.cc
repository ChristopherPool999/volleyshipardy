#include "battleship.h"

Battleship::Battleship() {
    grid1 = std::vector<std::vector<char>> (row + 1, std::vector<char>(col + 1, '~')); 
}

std::string Battleship::getTileOutput(int tileX, int tileY) const {
    int length = shipSizes[currentShip];
    int cursorX = cursorPostion.first;
    int cursorY = cursorPostion.second;

    if (direction == RIGHT && cursorY == tileY) {
         if (tileX >= cursorX && tileX <= cursorX + length) {
            return "  ";
         }
    } else if (direction == LEFT && cursorY == tileY) {
         if (tileX >= cursorX - length && tileX <= cursorX) {
            return "X  ";
         }
    } else if (direction == UP && cursorX == tileX) {
         if (tileY >= cursorY - length && tileY <= cursorY) {
            return "X  ";
         }   
    } else if (direction == DOWN && cursorX == tileX) {
         if (tileY >= cursorY && tileY <= cursorY + length ) {
            return "X  ";
         }  
    }
    std::string text = "";
    text.push_back(grid1[tileY][tileX]);
    text += "  ";
    return text;
}

void Battleship::printUI(int player) const {
    std::string left = (player == 1 ? "   YOU   " : "PLAYER 1");
    std::string right = (player == 2 ? "   YOU   " : "PLAYER 2");
    printw("           %s                %d             %s            \n", left.c_str(), 60, right.c_str());
    printw("___________________________________________________________________________\n");
    printw("  0  1  2  3  4  5  6  7  8  9       |       0  1  2  3  4  5  6  7  8  9  \n"  );

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
        printw("%s", (text += '\n').c_str());
    }
}

void Battleship::attack() {
    ;
}

void Battleship::handleMovement(int KEY) {
    if (KEY == A && cursorPostion.first > 0) {
        cursorPostion.first--;
    } else if (KEY == D && cursorPostion.first < col - 1) {
         cursorPostion.first++;
    } else if (KEY == S && cursorPostion.second < row - 1) {
        cursorPostion.second++;
    } else if (KEY == W && cursorPostion.second > 0) {
        cursorPostion.second--;
    }
}

void Battleship::handleDirection(int KEY) {
    if (KEY == RIGHT) {
        if ()
    } else if (KEY == LEFT) {
        
    } else if (KEY == DOWN) {
        
    } else if (KEY == UP) {
        
    }
}

std::vector<std::pair<int, int>> Battleship::getValidTiles(int direction = 0) {
    if (!direction) {
        for (int i = 0; i < shipSizes[currentShip]; i++) {

        }
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
        } else if (KEY == ENTER && getValidTiles().size() == shipSizes[currentShip]) {
            std::vector<std::pair<int, int>> tiles = getValidTiles();
            for (auto p : tiles) {
                grid1[p.first][p.second] = 'X';               
            }
            currentShip++;
            
            break;
        }
    }
}