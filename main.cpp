#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <string>
#include <random>
#include "service.h"
#include "handlers.h"

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
using u_char = unsigned char;

int Dispatcher(std::basic_ostream<wchar_t> &ostream, std::basic_istream<wchar_t> &istream,
		   std::map<std::wstring, int> &CommandMapping,
           std::map<std::wstring, std::wstring>& Help, std::vector<Matrix<MATRIX_T>> &MatrixSet,
           const std::wstring& Prompt) {

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
			} case 2: { // Красивый вывод
				Handlers::FormatOutputHandler<MATRIX_T>(MatrixSet, Arguments, ostream);
				break;									// Форматированный вывод матрицы
			} case 3: { // Стандартный ввод
				if (!Arguments[2].empty()) {
					auto file = Handlers::OpenIFileHandler(Arguments[2]);
					Handlers::InputHandler<MATRIX_T>(MatrixSet, Arguments, file);
					file.close();
				} else {
					Handlers::InputHandler<MATRIX_T>(MatrixSet, Arguments, istream);
					std::getline(istream, to_parse); // После ввода почему-то считывается пустая строка, это фикс
				}
				break;
			} case 4: { // Стандартный вывод
				if (!Arguments[2].empty()) {
					auto file = Handlers::OpenOFileHandler(Arguments[2]);
					Handlers::OutputHandler<MATRIX_T>(MatrixSet, Arguments, file);
					file.close();
				} else {
					Handlers::OutputHandler<MATRIX_T>(MatrixSet, Arguments, ostream);
				}
				break;
			} case 5: { // Execute
				auto script = Handlers::OpenIFileHandler(Arguments[1]);
				while (!script.eof()) {
					Dispatcher(ostream, script, CommandMapping, Help, MatrixSet, L"");
				}
				break;
			} case 6: { // Установка имени
				Handlers::SetNameHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 7: { // Заполнение
				Handlers::FillMatrixHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 8: {	// Переразметка
				Handlers::ResizeMatrixHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 9: {  // Умножение матриц
				Handlers::MatrixMultiplicationHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 10: { // Умножение самой матрицы (*=)
				Handlers::MatrixSelfMultiplicationHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 11: {	// Вывод детерминанта.
				Handlers::DeterminantHandler<MATRIX_T>(MatrixSet, Arguments, ostream);
				break;
			} case 12: { // Сложение со скаляром
				Handlers::AdditionByScalarHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 13: { // Вычитание скаляра. Так. Нахуя я делал две одинаковые функции. ЛОЛ
				Handlers::SubtractionByScalarHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 14: { // Умножение на скаляр
				Handlers::MultiplicationByScalarHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 15: { // Деление на скаляр
				Handlers::DivisionByScalarHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 16: { // Поэлементное сложение матриц
				Handlers::AdditionByMatrixHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 17: { // Поэлементное сложение матриц
				Handlers::SubtractionByMatrixHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 18: { // Поэлементное умножение матриц
				Handlers::MultiplicationByMatrixHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 19: { // Поэлементное деление матриц
				Handlers::DivisionByMatrixHandler<MATRIX_T>(MatrixSet, Arguments);
				break;
			} case 20: {
				Handlers::SubmatrixHandler<MATRIX_T>(MatrixSet, Arguments);
			} case 21: {
				ostream << L"Не пали контору." << std::endl;
				break;
			} case 22: {
				Handlers::CopyMatrixHandler(MatrixSet, Arguments);
				break;
			} case 23: {
				// Handlers::TransposeHandler(MatrixSet, Arguments);
				break;
			} case 24: {
                Handlers::HelpHandler(Arguments, Help, ostream);
                break;
			} case 25: { // Выход
				return -1;
			} case 26: { // Пустая строка
				break;
			} default: { // Команда не найдена
				ostream << L"Ошибка: Команда \"" << Command << L"\" не найдена." << std::endl;
			}
		}
	} catch (const std::invalid_argument &invalidArgument) { // Обработка исключения std::stoi
		ostream << L"Ошибка: Неверный аргумент в \"" << Arguments[0];
		for (size_t i = 1; !Arguments[i].empty() && i < Arguments.size(); i++) {
			ostream << " " << Arguments[i];
		}
		ostream << "\"." << std::endl;
	} catch (const std::exception &unknownException) { // Обычно ловит ошибки std::bad_allocation
		ostream << L"Неизвестное исключение: " << unknownException.what() << L"." << std::endl;
	} catch (ExceptionWithMessage& error) {
		ostream << L"Ошибка: " << error.get_error() << std::endl;  // Ошибки хендлеров
	}
	return 0;
}
// TODO: сделать нормальную обработку ошибок через std::exception

void StartUp(std::vector<std::wstring>& Settings, std::map<std::wstring, int> &CommandMapping,
             std::map<std::wstring, std::wstring>& Help, std::vector<Matrix<MATRIX_T>> &MatrixSet, const std::wstring& Prompt) {
	std::wstringstream ostream;
	std::wstringstream istream;

	for (std::wstring& str: Settings) {
		istream << str << std::endl;
	}

	Dispatcher(ostream, istream, CommandMapping, Help, MatrixSet, Prompt);
}

int main() {
	std::vector<std::wstring> on_startup = {L"execute /home/reenie/startup.msh"};
	setlocale(LC_CTYPE, "");

	std::vector<Matrix<MATRIX_T>> MatrixSet(10);

	auto& ostream = std::wcout;
	auto& istream = std::wcin;

	std::map<std::wstring, int> CommandMapping = {
			{L"list",			 1},
			{L"print", 		 2},
			{L"input", 		 3},
			{L"output", 		 4},
			{L"execute", 		 5},
			{L"name", 		 6},
			{L"fill", 		 7},
			{L"resize", 		 8},
			{L"multiply",  	 9},
			{L"multiplywith",	10},
			{L"determinant", 	11},
			{L"scalar+", 		12},
			{L"scalar-", 		13},
			{L"scalar*", 		14},
			{L"scalar/", 		15},
			{L"matrix+", 		16},
			{L"matrix-", 		17},
			{L"matrix*", 		18},
			{L"matrix/",  	19},
			{L"submatrix",  	20},
			{L"iddqd",	   	21},
			{L"copy",	   		22},
			{L"transpose",	23},
			{L"help",	   		24},
			{L"exit", 		25},
			{L"", 			26}
	};

    std::map<std::wstring, std::wstring> Help = {
            {L"list", 		L" -- вывод списка матриц"},
            {L"print", 		L" номер_матрицы [точность_вывода = 4] -- красивый вывод в консоль"},
            {L"input", 		L" номер_матрицы  [имя_файла] -- ввод файла"},
            {L"output", 		L" номер_матрицы [имя_файла] -- вывод матрицы"},
            {L"execute",		L" имя_файла -- исполнение файла"},
            {L"name", 		L" номер_матрицы псевдоним_матрицы -- задание имени матрицы. Затем можно использовать вместо номера"},
            {L"fill", 		L" режим_заполнения [границы диапазона, значение] -- заполнение матрицы"},
            {L"resize", 		L" номер_матрицы новый_размер1 новый_размер2 -- изменение размера"},
            {L"multiply", 	L" номер_матрицы1 номер_матрицы2 номер_матрицы3 -- умножение матриц 2 и 3, и запись в матрицу1"},
            {L"multiplywith",	L" номер_матрицы1 номер_матрицы2 -- умножение матрицы 1 на матрицу 2"},
            {L"determinant", 	L" номер_матрицы -- вывод детерминанта матрицы"},
            {L"scalar+", 		L" номер_матрицы скаляр -- сложение со скаляром"},
            {L"scalar-", 		L" номер_матрицы скаляр -- вычитание скаляра"},
            {L"scalar*", 		L" номер_матрицы скаляр -- умножение на скаляр"},
            {L"scalar/", 		L" номер_матрицы скаляр -- деление на скаляр"},
            {L"matrix+", 		L" номер_матрицы1 номер_матрицы2-- сложение с матрицы 1 с матрицей 2. Результат в матрице1"},
            {L"matrix-", 		L" номер_матрицы1 номер_матрицы2 -- вычитание матрицы 2 из матрицей 1. Результат в матрице1"},
            {L"matrix*", 		L" номер_матрицы1 номер_матрицы2 -- умножение матрицы 1 на матрицу 2. Результат в матрице1"},
            {L"matrix/", 		L" номер_матрицы1 номер_матрицы2 -- деление матрицы 1 на матрицу 2. Результат в матрице1"},
            {L"copy", 		L" номер_матрицы1 номер_матрицы2 -- копирование матрицы2 в матрицу1"},
            {L"submatrix", 	L" номер_матрицы1 номер_матрицы2 столбец строка -- нахождение подматрицы по координатам. Результат в матрице1"},
            {L"exit", 		L" -- выход"}
    };

	time_t timestamp = std::time(nullptr);
	ostream << "Mitrix v1.0 at " << std::ctime(&timestamp);
	ostream << "Matrices count: " << MatrixSet.size() << std::endl;
	if (!on_startup.empty()) {
		ostream << "On StartUp: " << on_startup[0] << std::endl;
		for (size_t i = 1; i < on_startup.size(); i++) {
			ostream << std::setw(12) << "" << on_startup[i] << std::endl;
		}
	}

	StartUp(on_startup, CommandMapping, Help, MatrixSet, L"");

	int run_code = 0;
	while (!run_code)
		run_code = Dispatcher(ostream, istream, CommandMapping, Help, MatrixSet, L">>> ");
}
// mitrix-cli */