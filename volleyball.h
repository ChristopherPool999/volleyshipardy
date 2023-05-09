#ifndef VOLLEYBALL_H
#define VOLLEYBALL_H

#include <set>
#include <bits/stdc++.h>
#include <vector>
#include <utility>

class Volleyball {
    std::vector<std::vector<std::string>> &questions;
	std::set<int> used = {};
	std::chrono::duration<double> timeLimit = std::chrono::duration<double>(10);
    std::chrono::duration<double> currTime = timeLimit - timeLimit;
    int player = 1;
	int questionNum = -1;
    std::string answer = "";
    void printUI(std::string, std::string);
    void findQuestion();
    void printQuestion() const;
    std::pair<bool, double> getResults();
    void swapPlayer();
    void showResults(std::pair<bool, double>&);
    int playGame();
public:
    Volleyball(std::vector<std::vector<std::string>> &questions);
    int startGame();
};
#endif // VOLLEYBALL_H 