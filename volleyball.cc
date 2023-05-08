#include <iostream>
#include <ncurses.h>
#include "volleyball.h"

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
	std::cout << "I Want To Play A Game 🤡\n\nPick the correct question or else... (use 1, 2, 3, 4)\n"
			<< questions[questionNum][0] << "\n\n"
			<< questions[questionNum][1] << "\n"
			<< questions[questionNum][2] << "\n"
			<< questions[questionNum][3] << "\n"
			<< questions[questionNum][4] << "\n";
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

int Volleyball::playGame() {
	while (true) {
		findQuestion();
		printQuestion();
		if (!isCorrect()) {
    		clear();
    		endwin();
			return (player == 2 ? 1 : 2);
		}
		swapPlayer();
    }
	return 0;
}
