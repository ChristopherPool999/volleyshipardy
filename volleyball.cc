#include <iostream>
#include <ncurses.h>
#include "volleyball.h"
#include <algorithm>

Volleyball::Volleyball(std::vector<std::vector<std::string>> &questions) : questions(questions) {}

int Volleyball::startGame() {
	initscr();
	clear();
	noecho();
	cbreak();
	std::srand(time(0));
	return playGame();
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
printw("I Want To Play A Game\n\nPick the correct question or else... (use 1, 2, 3, 4)\n"
            "%s\n\n%s\n%s\n%s\n%s\n",
            questions[questionNum][0].c_str(),
            questions[questionNum][1].c_str(),
            questions[questionNum][2].c_str(),
            questions[questionNum][3].c_str(),
            questions[questionNum][4].c_str());
}

bool Volleyball::isCorrect() {
	auto start = std::chrono::high_resolution_clock::now();
    
	std::future<int> future = std::async([](){
		return getch();
	});
    auto status = future.wait_for(timeLimit);
	if (status == std::future_status::timeout || questions[questionNum][future.get()] != answer) {
		return 0;
	} else {
		auto end = std::chrono::high_resolution_clock::now();
    	timeLimit = std::chrono::duration_cast<std::chrono::duration<double>> (end - start);
		return 1;
	}
}

void Volleyball::swapPlayer() {
	; //  implement later with networking
}

void Volleyball::showResults(bool isCorrect) {
    std::string result = (isCorrect ? "correct" : "incorrect");
    printw("You chose wrong :( The correct answer was %s\n\n", answer.c_str());
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int Volleyball::playGame() {
	while (true) {
		findQuestion();
		printQuestion();
		if (!isCorrect()) {
            showResults(false);
    		clear();
    		endwin();
			return (player == 2 ? 1 : 2);
		}
		swapPlayer();
        showResults(true);
        clear();
    }
	return 0;
}
