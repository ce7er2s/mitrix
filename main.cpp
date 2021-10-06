#include <iostream>
#include <filesystem>
#include <map>
#include "mitrix.h"
#include "service.h"

std::vector<std::wstring> ParseArguments(const std::wstring& to_parse) {

	std::vector<std::wstring> args(5, L"");
	size_t i = 0;

	std::wstringstream parser;  // поток-хранилище входной строки
	parser.str(to_parse); // Копирование строки в поток-хранилище строк, ок
	while (parser >> args[i])
		i++;

	for (auto& letter: args[0])
		letter = std::tolower(letter);

	return args;
}

typedef double MATRIX_T;

int Execute(std::basic_ostream<wchar_t> &ostream, std::basic_istream<wchar_t> &istream,
		   std::map<std::wstring, int> &commandMapping, std::map<int, std::wstring> &Exceptions,
		   std::vector<Matrix<MATRIX_T>> &matrixSet, const std::wstring& Prompt) {

	std::wstring to_parse;
	std::getline(istream, to_parse);

	auto args = ParseArguments(to_parse);
	auto command = args[0];

	int command_code = commandMapping[command];

	try {
		switch (command_code) {
			case 1: { // Вывод списка.
				Handlers::ListHandler(matrixSet, ostream);  // Вывод списка матриц.
				break;
			}
			case 2: { // Красивый вывод
				int32_t precision;
				int32_t index = std::stoi(args[1]) - 1;  // Ошибка перевода stoi отлавливается ниже
				Matrix<MATRIX_T>* matrix = Handlers::GetMatrixHandler(matrixSet, index);
				if (!args[2].empty()) //
					precision = std::stoi(args[2]);	// Ошибка перевода stoi отлавливается ниже
				else
					precision = 4; // Значение точности по умолчанию
				Handlers::FormatOutputHandler(*matrix, ostream, precision);
				break;									// Форматированный вывод матрицы
			}
			case 3: { // Стандартный ввод
				int32_t index = std::stoi(args[1]) - 1;  // Ошибка перевода stoi отлавливается ниже
				Matrix<MATRIX_T>* matrix = Handlers::GetMatrixHandler(matrixSet, index);
				if (!args[2].empty()) {
					std::filesystem::path path(args[2]);
					std::basic_ifstream<wchar_t> file(path);  // а если эта срань сломается? Что делать? Файл-то незакрыт.
					Handlers::InputHandler(*matrix, file);
					file.close();
				} else {
					Handlers::InputHandler(*matrix, istream);
					std::getline(istream, to_parse); // После ввода почему-то считывается пустая строка, это фикс
				}
				break;
			}
			case 4: { // Стандартный вывод
				int32_t index = std::stoi(args[1]) - 1;  // Ошибка перевода stoi отлавливается ниже
				Matrix<MATRIX_T>* matrix = Handlers::GetMatrixHandler(matrixSet, index);
				if (!args[2].empty()) {
					std::filesystem::path path(args[2]);
					std::basic_ofstream<wchar_t> file(path);  //
					Handlers::OutputHandler(*matrix, file);
					file.close();
				} else {
					Handlers::OutputHandler(*matrix, ostream);
				}
				break;
			}
			case 5: {
				std::filesystem::path path(args[1]);
				std::basic_ifstream<wchar_t> script(path);
				if (!script.is_open()) {
					throw 2; // см Exceptions;
				}
				while (!script.eof()) {
					Execute(ostream, script, commandMapping, Exceptions, matrixSet, L"");
				}
				break;
			}
			case 6: {
				return -1;
			}
			case 7: {
				break;
			}
			default:
				ostream << L"WRONG COMMAND \"" << command << "\"." << std::endl;
		}
	} catch (const std::invalid_argument &invalidArgument) { // Обработка исключения std::stoi
		ostream << "WRONG ARGUMENT in command: \"" << to_parse << "\"." << std::endl;
	} catch (const std::exception &unknownException) { // Обычно ловит ошибки std::bad_allocation
		ostream << "UNKNOWN EXCEPTION: " << unknownException.what() << "." << std::endl;
	} catch (int &errorCode) {
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
		matrix.FillStorage('r', 0, -11, 1);
	}

	auto &ostream = std::wcout;
	auto &istream = std::wcin;

	std::map<std::wstring, int> commandMapping = {
			{L"list",1},
			{L"print", 2},
			{L"input", 3},
			{L"output", 4},
			{L"execute", 5},
			{L"exit", 6},
			{L"", 7}
	};

	std::map<int, std::wstring> Exceptions = {
			{1, L"NEGATIVE ARGUMENT"},
			{2, L"FILE NOT FOUND"},
			{3, L"ZERO LENGTH"},
			{4, L"DETERMINANT IS ZERO"},
			{5, L"MULTIPLICATION IMPOSSIBLE"},
			{6, L"MATRIX DOES NOT EXIST"}
	};

	while (!Execute(ostream, istream, commandMapping, Exceptions, matrixSet, L">>> "))
		;

}
// mitrix-cli */