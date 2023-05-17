#include "volleyball.h"

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

void Volleyball::printQuestion() {
    int printCol = 40;
    std::string playerStr = player == 1 ? "YOU!!!" : "PLAYER 2!!!";
    mvaddstr(0, 45, playerStr.c_str());
    char output[200];
    sprintf(output,
        "Pick the correct question in - %f - or else...", timeLimit.count()
    );
    mvaddstr(2, 40, output);
    printRow = 3;
    for (auto c : questions[questionNum][0]) {
        if (c == '\n') {
            printCol = 40;
            printRow++;
            continue;
        }
        mvaddch(printRow, printCol++, c);
    }
    std::string question1 = "1.) " + questions[questionNum][1];
    std::string question2 = "2.) " + questions[questionNum][2];
    std::string question3 = "3.) " + questions[questionNum][3];
    std::string question4 = "4.) " + questions[questionNum][4];
    mvaddstr(printRow += 2, 40, question1.c_str());
    mvaddstr(++printRow, 40, question2.c_str());
    mvaddstr(++printRow, 40, question3.c_str());
    mvaddstr(++printRow, 40, question4.c_str());
}

std::pair<bool, double> Volleyball::getResults() {
	auto start = std::chrono::high_resolution_clock::now();
    int choice = -1;
    while (choice != 1 && choice != 2 && choice != 3 && choice != 4) {
        choice = getch() - '0';
    }
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
    char output[200];
    sprintf(output, 
        "You chose %s. The correct answer was %s",
            faceEmoji.c_str(), answer.c_str()
    );
    mvaddstr(printRow += 2, 40, output);
    sprintf(output, 
        "With a time limit left of... %s", std::to_string(results.second).c_str()
    );
    mvaddstr(++printRow, 40, output);
    refresh();
}

std::pair<bool, double> Volleyball::playGame(int player, double timeLimit) {
    this->player = player;
    this->timeLimit = std::chrono::duration<double>(timeLimit);
	std::srand(time(0));
    findQuestion();

    printQuestion();
    std::pair<bool, double> results = getResults();
    showResults(results);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return results;
}
