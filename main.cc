#include "volleyball.h"

int main() {
	std::ifstream file("foo.txt");
	std::string line;
	std::vector<std::vector<std::string>> questions;
	int i = 0;
	int k = 0;
	while (std::getline(file, line)) {
		if (k == 0) {
			std::vector<std::string> temp(5);
			questions.push_back(temp);
		}		
		questions[questions.size() - 1][k] = line;
		k++;
		if (k == 5) {
			i++;
			k = 0;
            std::getline(file, line);
		}
	}

    for (auto &v : questions) {
        for (auto &s : v) {
            for (int i = 0; i < s.size(); i++) {
                if (int(s[i]) == 92) {
                    s[i] = ' ';
                    s[i+1] = '\n';
                }
            }
        }
    }

    for (auto &v : questions) {
        if (v[0][v[0].size() - 1] != '\n') {
            v[0].push_back('\n');
        }
    }

    Volleyball game = Volleyball(questions);
    game.startGame();
}