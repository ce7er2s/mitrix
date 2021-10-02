#include <iostream>
#include <map>
#include "mitrix.h"
#include "service.h"

typedef double MATRIX_T;

int Parse(std::basic_ostream<wchar_t> &ostream, std::basic_istream<wchar_t> &istream,
		   std::map<std::wstring, int> &commandMapping, std::vector<Matrix<MATRIX_T>> &matrixSet) {
	std::wstringstream parser;
	std::vector<std::wstring> args(10);
	std::wstring str_to_parse;
	std::wstring command;
	std::wstring arg;
	int i = 0;

	ostream << L">>> ";
	std::getline(istream, str_to_parse);
	parser.str(str_to_parse);
	while (parser >> args[i]) {
		i++;
	}

	command = args[0];
	for (auto &ch: command)
		ch = std::tolower(ch);
	int command_code = commandMapping[command];

	switch (command_code) {
		case 1: {
			Handlers::ListHandler(matrixSet, ostream);
			break;
		}
		case 2: {
			Handlers::FormatOutputHandler(matrixSet[std::stoi(args[1])], ostream, std::stoi(args[2]));
			break;
		}
		case 3: {
			return -1;
		}
		default:
			ostream << L"Wrong command \""  << command << "\"" << std::endl;
	}
	return 0;
}


int main() { // TODO: Вынести сервисные функции в service.cpp
	setlocale(LC_CTYPE, "");

	std::vector<Matrix<MATRIX_T>> matrixSet(10);
	for (Matrix<MATRIX_T> &matrix: matrixSet) {
		matrix.resizeTo(10, 10);
		matrix.fillStorage('r', 0, -11, 1);
	}

	std::map<std::wstring, int> commandMapping;
	commandMapping[L"list"] = 1;
	commandMapping[L"print"] = 2;
	commandMapping[L"exit"] = 3;
	auto &ostream = std::wcout;
	auto &istream = std::wcin;

	int run_code = 0;
	while (run_code == 0) {
		run_code = Parse(ostream, istream, commandMapping, matrixSet);
	}
}
	/*std::wfstream output("output.txt");
	std::vector<Matrix<MATRIX_T>> matrix_set(10);
	for (Matrix<MATRIX_T> &matrix: matrix_set) {
		matrix.resizeTo(10, 10);
		matrix.fillStorage('r', 0, -11, 1);
	}
	Handlers::InputHandler<MATRIX_T>(matrix_set[0]);
	Handlers::FormatOutputHandler<MATRIX_T>(matrix_set[0]);
	Handlers::ListHandler<MATRIX_T>(matrix_set, std::wcout);
}

// mitrix-cli */