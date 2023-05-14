#include <iostream>
#include <ncurses.h>
#include "volleyball.h"
#include <algorithm>

using durationDBL = std::chrono::duration<double>; 

Volleyball::Volleyball(std::ifstream &file) {
	std::string line;

	for (int i = 0; std::getline(file, line); ) {
        if (i == 5) {
            i = 0;
            continue;
        }
		if (i == 0) {
			std::vector<std::string> temp(5);
			questions.push_back(temp);
		}		
		questions[questions.size() - 1][i++] = line;
	}
    for (auto &v : questions) {
        for (auto &s : v) {
            for (int i = 0; i < int(s.size()); i++) {
                if (int(s[i]) == 92) {
                    s[i] = ' ';
                    s[i+1] = '\n';
                }
            }
        }
        if (v[0][v[0].size() - 1] != '\n') {
            v[0].push_back('\n');
        }
    }
}

void Volleyball::findQuestion() {
	while (used.find(questionNum) != used.end() || questionNum == -1) {
		questionNum = std::rand() % questions.size();
	}
	used.insert(questionNum);
	answer = questions[questionNum][1];
    for (int i = 1; i < 5; i++) {
        std::swap(questions[questionNum][i], questions[questionNum][(std::rand() % 4) + 1]);
    }
}

void Volleyball::printQuestion() const {
    printw("Player %d, I Want To Play A Game\nPick the correct question or else... (use 1, 2, 3, 4)\n\n"
        "%s\n\n%s\n%s\n%s\n%s\nTimer: %f\n\n",
        player,
        questions[questionNum][0].c_str(),
        questions[questionNum][1].c_str(),
        questions[questionNum][2].c_str(),
        questions[questionNum][3].c_str(),
        questions[questionNum][4].c_str(),
        timeLimit.count());
}

std::pair<bool, double> Volleyball::getResults() {
	auto start = std::chrono::high_resolution_clock::now();
    int choice = getch() - '0';
	auto end = std::chrono::high_resolution_clock::now();
    auto temp = timeLimit;
    timeLimit = std::chrono::duration_cast<durationDBL>(end - start);
    if (choice < 0 || choice > 4 || timeLimit.count() >= temp.count() || questions[questionNum][choice] != answer) {
        return std::make_pair(0, temp.count() - timeLimit.count());
    }
    return std::make_pair(1, temp.count() - timeLimit.count());
}

void Volleyball::showResults(std::pair<bool, double> &results) {
    std::string faceEmoji = (results.first ? "correct :)" : "incorrect :(");
    printw("You chose %s. The correct answer was %s\nwith a time left of... : %s\n\n",
            faceEmoji.c_str(), answer.c_str(), std::to_string(results.second).c_str());
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int Volleyball::playGame() {
	std::srand(time(0));
	while (true) {
		findQuestion();
        printQuestion();
        std::pair<bool, double> results = getResults();
        showResults(results);
		if (!results.first) {
    		clear();
    		endwin();
			return (player == 2 ? 1 : 2);
		}
        player = (player == 2 ? 1 : 2);
        clear();
    }
	return 1;
}