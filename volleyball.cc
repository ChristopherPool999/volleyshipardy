#include <iostream>
#include <ncurses.h>
#include "volleyball.h"
#include <algorithm>

using durationDBL = std::chrono::duration<double>; 

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
    printw("Player %d, I Want To Play A Game\n\nPick the correct question or else... (use 1, 2, 3, 4)\n"
        "%s\n\n%s\n%s\n%s\n%s\n",
        player,
        questions[questionNum][0].c_str(),
        questions[questionNum][1].c_str(),
        questions[questionNum][2].c_str(),
        questions[questionNum][3].c_str(),
        questions[questionNum][4].c_str());
}

bool Volleyball::isCorrect() {
	auto start = std::chrono::high_resolution_clock::now();
    
    // auto updateTimer = [&start](durationDBL limit, durationDBL &curr, durationDBL &start) -> void {
    //     while (curr < limit) {
    //         auto end = std::chrono::high_resolution_clock::now();
    //         auto  = std::chrono::duration_cast<durationDBL> (end - start);
    //     }
    // };
    // std::thread t([](){
    //     while (currTme)
    // });

	// std::future<void> timeExceeded = std::async([this]() {
	// 	return std::this_thread::sleep_for(timeLimit);
	// });
	// std::future<int> input = std::async([](){
	// 	return getch();
	// });
    // auto inputStatus = input.wait_for(timeLimit);
    // auto timeStatus = timeExceeded.wait_for(timeLimit);

    // if (input.get()) {
	// 	auto end = std::chrono::high_resolution_clock::now();
    // 	timeLimit = std::chrono::duration_cast<durationDBL> (end - start);
    //     return 1;
    // } else if (timeStatus == std::future_status::timeout || inputStatus == std::future_status::timeout) {
    //     return 0;
    // }
    int choice = getch() - '0';
	auto end = std::chrono::high_resolution_clock::now();
    timeLimit = std::chrono::duration_cast<durationDBL>(timeLimit - (end - start));
    return timeLimit.count() > 0 && questions[questionNum][choice] == answer;
}

void Volleyball::swapPlayer() {
	player = (player == 2 ? 1 : 2);
}

void Volleyball::showResults(bool isCorrect) {
    std::string result = (isCorrect ? "correct :)" : "incorrect :(");
    std::string timeLeft = std::to_string(timeLimit.count());
    printw("You chose %s. The correct answer was %s\nTime Left: %s\n", result.c_str(), answer.c_str(), timeLeft.c_str());
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void Volleyball::printUI(std::string question = "", std::string questionResult = "") {
    // auto time = std::chrono::duration_cast<durationDBL>(timeLimit - currTime).count();
    // std::string strTime = std::to_string(time);
    // if (strTime == "0") {
    //     strTime == "";
    // }
    // printw("\t\t\t\tVOLLYBALL\n%s\n%s\n%s", question.c_str(), questionResult.c_str(), strTime.c_str());
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
