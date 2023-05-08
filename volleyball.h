#ifndef VOLLEYBALL_H
#define VOLLEYBALL_H

#include <set>
#include <bits/stdc++.h>
#include <vector>

class Volleyball {
    std::vector<std::vector<std::string>> &questions;
	std::set<int> used = {};
	std::chrono::duration<double> timeLimit = std::chrono::duration<double>(5);
    int player = 1;
	int questionNum = -1;
    std::string answer = "";
    void findQuestion();
    void printQuestion() const;
    bool isCorrect();
    void swapPlayer();
    void showResults(bool);
    int playGame();
public:
    Volleyball(std::vector<std::vector<std::string>> &questions);
    int startGame();
};
#endif // VOLLEYBALL_H 