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
	std::chrono::duration<double> timeLimit = std::chrono::duration<double>(10);
    std::chrono::duration<double> currTime = timeLimit - timeLimit;
    int player = 1;
	int questionNum = -1;
    std::string answer = "";
    void findQuestion();
    void printQuestion() const;
    std::pair<bool, double> getResults();
    void showResults(std::pair<bool, double>&);
public:
    Volleyball(std::ifstream&);
    int playGame();
};
#endif // VOLLEYBALL_H 