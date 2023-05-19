#ifndef VOLLEYBALL_H
#define VOLLEYBALL_H

#include <set>
#include <vector>
#include <utility>
#include <istream>
#include <ncurses.h>
#include <curses.h>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

#define COLOR_NORMAL 8
#define COLOR_PLAYER 9

class Volleyball {
    std::vector<std::vector<std::string>> questions;
	std::set<int> used = {};
	std::chrono::duration<double> timeLimit;
    int player;
    bool isPlaying;
    int printRow = 0;
	int questionNum = -1;
    std::string answer = "";
    void findQuestion();
    void printQuestion();
    std::pair<bool, double> getResults();
public:
    Volleyball(std::ifstream &, int);
    std::pair<bool, double> playGame(double, bool isPlaying);
    void showResults(std::pair<bool, double>&);
};
#endif // VOLLEYBALL_H 

