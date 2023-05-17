#ifndef VOLLEYBALL_H
#define VOLLEYBALL_H

#include <boost/asio.hpp>
#include <set>
#include <vector>
#include <utility>
#include <istream>
#include <ncurses.h>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

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
    boost::asio::ip::tcp::iostream &stream;
    void showResults(std::pair<bool, double>&);
public:
    Volleyball(std::ifstream &, boost::asio::ip::tcp::iostream&);
    std::pair<bool, double> playGame(int, double);
};
#endif // VOLLEYBALL_H 
