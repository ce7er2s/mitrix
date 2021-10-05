#include <iostream>
#include <map>
#include "mitrix.h"
#include "service.h"


typedef double MATRIX_T;

int Parse(std::basic_ostream<wchar_t> &ostream, std::basic_istream<wchar_t> &istream,
		   std::map<std::wstring, int> &commandMapping, std::map<int, std::wstring> &Exceptions,
		   std::vector<Matrix<MATRIX_T>> &matrixSet) {

	std::wstringstream parser;  // поток-хранилище входной строки
	std::vector<std::wstring> args(10);	// вектор для аргументов
	std::wstring str_to_parse;				// буфер входной строки
	std::wstring command;					// отдельная строка для первого аргумента
	std::wstring arg;
	int i = 0;

	ostream << ">>> ";  // приглашение для ввода
	std::getline(istream, str_to_parse); // получение всей строки из ostream.
	parser.str(str_to_parse); // Копирование строки в поток-хранилище строк, ок
	while (parser >> args[i]) { // Запись аргументов из потока
		i++;
	}

	command = args[0];
	for (auto &ch: command)  // Костыль регистронезависимости
		ch = std::tolower(ch);
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
				Handlers::InputHandler(*matrix, istream);
				std::getline(istream, str_to_parse); // После ввода почему-то считывается пустая строка, это фикс
				break;
			}
			case 4: { // Стандартный вывод
				int32_t index = std::stoi(args[1]) - 1;  // Ошибка перевода stoi отлавливается ниже
				Matrix<MATRIX_T>* matrix = Handlers::GetMatrixHandler(matrixSet, index);
				Handlers::OutputHandler(*matrix, ostream);
				break;
			}
			case 5: {
				return -1;
			}
			case 6: {
				break;
			}
			default:
				ostream << L"WRONG COMMAND \"" << command << "\"." << std::endl;
		}
	} catch (const std::invalid_argument &invalidArgument) { // Обработка исключения std::stoi
		ostream << "WRONG ARGUMENT in command: \"" << str_to_parse << "\"." << std::endl;
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
		matrix.resizeTo(0, 0);
		matrix.fillStorage('r', 0, -11, 1);
	}

	std::map<std::wstring, int> commandMapping = {
			{L"list",1},
			{L"print", 2},
			{L"input", 3},
			{L"output", 4},
			{L"exit", 5},
			{L"", 6}
	};

	std::map<int, std::wstring> Exceptions = {
			{1, L"NEGATIVE ARGUMENT"},
			{2, L"FILE NOT FOUND"},
			{3, L"ZERO LENGTH"},
			{4, L"DETERMINANT IS ZERO"},
			{5, L"MULTIPLICATION IMPOSSIBLE"},
			{6, L"MATRIX DOES NOT EXIST"}
	};

	auto &ostream = std::wcout;
	auto &istream = std::wcin;

	while (!Parse(ostream, istream, commandMapping, Exceptions, matrixSet))
		;

}
// mitrix-cli */