#ifndef VOLLEYBALL_H
#define VOLLEYBALL_H

#include <set>
#include <bits/stdc++.h>
#include <vector>
#include <utility>
#include <istream>

class Volleyball {
    std::vector<std::vector<std::string>> questions;
	std::set<int> used = {};
	std::chrono::duration<double> timeLimit;
    int player = 1;
    int printRow = 0;
	int questionNum = -1;
    std::string answer = "";
    void findQuestion();
    void printQuestion();
    std::pair<bool, double> getResults();
    void showResults(std::pair<bool, double>&);
public:
    Volleyball(std::ifstream&);
    std::pair<bool, double> playGame(int, double);
};
#endif // VOLLEYBALL_H 