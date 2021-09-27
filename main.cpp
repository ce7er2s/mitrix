#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <ctime>


#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix
static std::vector<Matrix<float>> matrix_set(10);
static std::map<std::string, int> commands = {
		{"list", 1}
};

[[noreturn]] void executor (std::istream* _ifstream = &std::cin, std::ostream* _ofstream = &std::cout, std::string prompt = ">>> ") {

	std::vector<std::string> params = {};
	std::string raw_string;
	int command_code = -1;
	while (true) {
		std::cout << prompt;
		std::vector<std::string> params = {};
		std::string raw_string;
		*_ifstream >> raw_string;
		command_code = commands[raw_string];
		switch (command_code) {
			case 1: {
				for (uint32_t i = 0; i < matrix_set.size(); i++) {
					std::cout << std::to_string(i) << " " << matrix_set[i].rows << " " << matrix_set[i].columns << std::endl;
				}
			}
		}
	}
}

int main() { // TODO: Вынести сервисные функции в service.cpp
	bool _exit = true;
	while (_exit) {
		std::time_t timestamp = std::time(nullptr);
		std::cout << "Start at: " << std::ctime(&timestamp);
		std::cout << "Mitrix v0.1 with float matrix type." << std::endl;
		try {
			executor();
		} catch (int exit_code) {
			if (exit_code == 1) {
				std::cout << "Do you surely want to exit? [Y/n]";
				int answer = getchar();
				if ((answer == 'y') || (answer == 'Y')) {
					_exit = false;
				} else {
					_exit = true;
				}
			}
		}
		catch (...) {
			std::cout << "Program suddenly dropped dead. Recover? [y/N]: ";
			int answer = getchar();
			if ((answer == 'n') || (answer == 'N')) {
				_exit = false;
			} else {
				_exit = true;
			}
		}
		std::istream *_istream = &std::cin;
		std::ostream *_ostream = &std::cout;
	}
}

// mitrix-cli