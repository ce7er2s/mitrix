#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <string>
#include <random>
#include "mitrix.h"
#include "service.h"

std::vector<std::wstring> ParseArguments(const std::wstring& to_parse) {

	std::vector<std::wstring> args(10, L"");
	size_t i = 0;

	std::wstringstream parser;  // поток-хранилище входной строки
	parser.str(to_parse); // Копирование строки в поток-хранилище строк, ок
	while (parser >> args[i] && i < 10)
		i++;

	for (auto& letter: args[0])
		letter = std::tolower(letter);

	return args;
}

using MATRIX_T = double;

int Dispatcher(std::basic_ostream<wchar_t> &ostream, std::basic_istream<wchar_t> &istream,
		   std::map<std::wstring, int> &CommandMapping, std::map<int, std::wstring> &Exceptions,
		   std::vector<Matrix<MATRIX_T>> &MatrixSet, const std::wstring& Prompt) {

	ostream << Prompt;
	
	std::wstring to_parse;
	std::getline(istream, to_parse);
	
	auto Arguments = ParseArguments(to_parse);
	auto Command = Arguments[0];
	
	int command_code = CommandMapping[Command];
	
	try {
		switch (command_code) {
			case 1: { // Вывод списка.
				Handlers::ListHandler<MATRIX_T>(MatrixSet,Arguments, ostream);  // Вывод списка матриц.
				break;
			}
			case 2: { // Красивый вывод
				Handlers::FormatOutputHandler<MATRIX_T>(MatrixSet, Arguments, ostream);
				break;									// Форматированный вывод матрицы
			}
			case 3: { // Стандартный ввод
				if (!Arguments[2].empty()) {
					auto file = Handlers::OpenIFileHandler(Arguments[2]);
					Handlers::InputHandler<MATRIX_T>(MatrixSet, Arguments, file);
					file.close();
				} else {
					Handlers::InputHandler<MATRIX_T>(MatrixSet, Arguments, istream);
					std::getline(istream, to_parse); // После ввода почему-то считывается пустая строка, это фикс
				}
				break;
			}
			case 4: { // Стандартный вывод
				if (!Arguments[2].empty()) {
					auto file = Handlers::OpenOFileHandler(Arguments[2]);
					Handlers::OutputHandler<MATRIX_T>(MatrixSet, Arguments, file);
					file.close();
				} else {
					Handlers::OutputHandler<MATRIX_T>(MatrixSet, Arguments, ostream);
				}
				break;
			}
			case 5: {
				auto script = Handlers::OpenIFileHandler(Arguments[1]);
				while (!script.eof()) {
					Dispatcher(ostream, script, CommandMapping, Exceptions, MatrixSet, L"");
				}
				break;
			}
			case 6: {
				Handlers::SetNameHandler<MATRIX_T>(MatrixSet, Arguments, ostream);
				break;
			}
			case 7: {
				Handlers::FillMatrixHandler<MATRIX_T>(MatrixSet, Arguments, ostream);
				break;
			}
			case 8: {
				Handlers::ResizeMatrixHandler<MATRIX_T>(MatrixSet, Arguments, ostream);
				break;
			}
			case 9: {
				ostream << "NOT IMPLEMENTED" << std::endl;
			}
			case 10: {
				return -1;
			}
			case 11: {
				break;
			}
			default:
				ostream << L"WRONG COMMAND \"" << Command << "\"." << std::endl;
		}
	} catch (const std::invalid_argument &invalidArgument) { // Обработка исключения std::stoi
		ostream << "WRONG ARGUMENT in command: \"" << to_parse << "\"." << std::endl;
	} catch (const std::exception &unknownException) { // Обычно ловит ошибки std::bad_allocation
		ostream << "UNKNOWN EXCEPTION: " << unknownException.what() << "." << std::endl;
	} catch (ERRORS& errorCode) {
		ostream << Exceptions[errorCode] << std::endl;  // Ошибки хендлеров
	}
	return 0;
}
// TODO: сделать нормальную обработку ошибок через std::exception

int main() {
	setlocale(LC_CTYPE, "");

	std::vector<Matrix<MATRIX_T>> matrixSet(10);
	for (Matrix<MATRIX_T> &matrix: matrixSet) {
		matrix.ResizeTo(0, 0);
		//matrix.FillStorage('r', 0, -10, 10);
	}

	auto& ostream = std::wcout;
	auto& istream = std::wcin;

	std::map<std::wstring, int> CommandMapping = {
			{L"list",1},
			{L"print", 2},
			{L"input", 3},
			{L"output", 4},
			{L"execute", 5},
			{L"name", 6},
			{L"fill", 7},
			{L"resize", 8},
			{L"multiply", 9},
			{L"exit", 10},
			{L"", 11}
	};

	std::map<int, std::wstring> Exceptions = {
			{1, L"NEGATIVE ARGUMENT"},
			{2, L"FILE NOT FOUND"},
			{3, L"ZERO LENGTH"},
			{4, L"DETERMINANT IS ZERO"},
			{5, L"MULTIPLICATION IMPOSSIBLE"},
			{6, L"MATRIX DOES NOT EXIST"},
			{7, L"UNKNOWN ERROR"},
			{8, L"INVALID ARGUMENT"},
			{9, L"NAME ALREADY EXISTS"}
	};

	time_t timestamp = std::time(nullptr);
	ostream << "Mitrix v1.0 at " << std::ctime(&timestamp);

	int run_code = 0;
	while (!run_code)
		run_code = Dispatcher(ostream, istream, CommandMapping, Exceptions, matrixSet, L">>> ");
}
// mitrix-cli */