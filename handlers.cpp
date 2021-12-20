#include <utility>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-exception-baseclass"
#pragma ide diagnostic ignored "misc-throw-by-value-catch-by-reference"
//
// Created by reenie on 27.09.2021.
//


class ExceptionWithMessage: std::exception {
private:
	std::wstring _error_msg;
public:
	explicit ExceptionWithMessage (std::wstring error_msg) {
		this->_error_msg = std::move(error_msg);
	}

	std::wstring get_error() {
		return this->_error_msg;
	}
};

template <typename T> void Handlers::ListHandler(
		std::vector<Matrix<T>>& MatrixSet,  std::vector<std::wstring>& Arguments, std::basic_ostream<wchar_t>& ostream) {
	ostream << L"\n" << " ";
	ostream << std::setw(2) << L"#" << "|";  // Отрисовка шапки
	ostream << std::setw(5) << L"ROWS" << "|";
	ostream << std::setw(8) << L"COLUMNS" << "|";
	ostream << std::setw(14) << L"STORAGE" << "|";
	ostream << std::setw(5) << L"NAME" << L"\n" <<  " ";
	ostream.width(28);
	ostream.fill('-');	// Отрисовка разделителя
	ostream << std::setw(2) << "" << "+";
	ostream << std::setw(5) << "" << "+";
	ostream << std::setw(8) << "" << "+";
	ostream << std::setw(14) << "" << "+";
	ostream << std::setw(16) << "" << L"\n";
	ostream.fill(' ');
	uint32_t i = 1;
	for (Matrix<T>& matrix: MatrixSet) { // Отрисовка инфы о каждой матрицы
		ostream << " ";
		ostream << std::setw(2) << i++ << "|";
		ostream << std::setw(5) << matrix.rows << "|";
		ostream << std::setw(8) << matrix.columns << "|";
		ostream << std::setw(8) << sizeof(T) * matrix.rows * matrix.columns << std::setw(6) << L"bytes" << "|";
		ostream << " ";
		if (!matrix.name.empty()) {
			ostream << matrix.name << L"\n";
		}
		else {
			ostream << L"NONE" << L"\n";
		}
	}
	ostream << L"\n";
}

template <typename T> void Handlers::InputHandler(
		std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments, std::basic_istream<wchar_t>& istream) {
	auto& matrix = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);

	int64_t rows = 0; // Длина и ширина вводятся первыми
	int64_t columns = 0;
	istream >> rows;	// Ввод из потока
	istream >> columns;
	if ((rows == 0) || (columns == 0))		// Обработка ошибок
		throw ExceptionWithMessage(L"Нулевые размеры матрицы.");
	if ((rows < 0) || (columns < 0)) {
		throw ExceptionWithMessage(L"Отрицательные размеры матрицы.");
	}
	matrix.ResizeTo(rows, columns); // Переразметка матрицы
	for (uint32_t i = 0; i < rows; i++) {
		for (uint32_t j = 0; j < columns; j++) {
			istream >> matrix._storage[i][j]; // Заполнение
		}
	}
}

template <typename T> void Handlers::OutputHandler(
		std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments, std::basic_ostream<wchar_t>& ostream) {
	auto& matrix = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	ostream << matrix.rows << " " << matrix.columns << L"\n";
	for (uint32_t i = 0; i < matrix.rows; i++) {
		for (uint32_t j = 0; j < matrix.columns - 1; j++) {
			ostream << matrix[i][j] << " ";
		}
		ostream << matrix[i][matrix.columns-1] << L"\n";
	}
}

template <typename T> void Handlers::FormatOutputHandler(
		std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments, std::basic_ostream<wchar_t>& ostream) {
	auto& matrix = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]); // Получение ссылки
	if (matrix.rows == 0 || matrix.columns == 0) { // проверка на нулевую длину
		throw ExceptionWithMessage(L"Нулевые размеры матрицы.");
	}

	unsigned char precision = 4;  // Стандартное значение
	if (!Arguments[2].empty()) { // Если второй аргумент не пустой, попытаться поменять точность. В случае ошибки дернуть исключение
		try {
			precision = std::stoi(Arguments[2]);
		} catch (std::invalid_argument InvalidArgument) {
			throw ExceptionWithMessage(L"Неверный аргумент точности");
		} catch (...) {
			throw; // Заглушка для других проблем
		}
	}

	size_t element_max_size = 0;  // Для правильной конвертации таблицы нужны размеры точные
	size_t row_max_size = std::to_string(matrix._storage.size()).length();
	size_t columns_max_size = std::to_string(matrix._storage[0].size()).length();
	std::basic_stringstream<wchar_t> printer;
	printer << std::setprecision(precision) << std::fixed;

	for (std::vector<T> row: matrix._storage) {  // Поиск максимально длинного элемента
		for (T element: row) {
			printer << element;
			element_max_size = std::max(element_max_size, printer.str().length());
			printer.str(L"");
		}
		columns_max_size = std::max(columns_max_size, std::to_string(row.size()).length());
	}

	element_max_size = std::max(element_max_size, columns_max_size);  // Поиск наибольшего
	auto cell_size = static_cast<int32_t>(element_max_size) + 1;                                  // И его общая запись
	auto rows_size = static_cast<unsigned char>(columns_max_size) + 1;
	// +1 для отступа.
	ostream << std::fixed << std::setprecision(precision);
	ostream << L"\n" << " ";

	ostream << std::setw(rows_size) << "";
	for (uint32_t i = 0; i < matrix.columns; i++) {  // Отрисовка шапки с индексами: 1| 2| 3|
		ostream << " |" << std::setw(cell_size) << i+1;
	}
	ostream << L"\n";

	for (uint32_t i = 0; i < matrix.rows; i++) {  // Отрисовка полоски и рядов. Одна итерация -- полоска и ряд элементов
		ostream << " ";
		ostream.fill('-'); // Отрисовка полосок
		ostream << std::setw(rows_size+1) << "";
		for (uint32_t j = 0; j < matrix.columns-1; j++)
			ostream << '+' << std::setw(cell_size+1) << "";
		ostream << "+" << std::setw(cell_size) << "";
		ostream.fill(' ');
		ostream << L"\n";

		ostream << " ";
		ostream << std::setw(rows_size) << i+1 << " |";  // Отрисовка элементов
		ostream << std::setw(cell_size) << matrix[i][0];
		for (uint32_t j = 1; j < matrix.columns; j++) {
			ostream << " |" << std::setw(cell_size) << matrix._storage[i][j];
		}
		ostream << L"\n";
	}
	ostream << L"\n";
}

template <typename T> void Handlers::DeterminantHandler(
		std::vector<Matrix<T>>& MatrixSet,  std::vector<std::wstring>& Arguments, std::basic_ostream<wchar_t>& ostream) {
	auto& matrix = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]); // Получение ссылки на матрицу
	unsigned char precision = 4;
	if (matrix.rows != matrix.columns) {	// TODO: проверь в отладчике, что копирует, а что передает ссылку auto или auto&
		throw ExceptionWithMessage(L"Детерминант отсутствует, количество строк и рядов не совпадает.");	// Простая проверка
	}
	if (matrix.rows == 0 || matrix.columns == 0) {
		throw ExceptionWithMessage(L"Один из размеров матрицы равен нулю.");
	}
	if (!Arguments[2].empty()) {
		precision = std::stoul(Arguments[2]);
	}
	ostream << std::setprecision(precision);
	ostream << L"Детерминант равен " << matrix.DeterminantOf() << L"." << L"\n";
}

void Handlers::HelpHandler(std::vector<std::wstring>& Arguments,
        std::map<std::wstring, std::wstring> Help, std::basic_ostream<wchar_t>& ostream) {
    if (!Arguments[1].empty()) {
        std::wstring lower_arg1 = Arguments[1];
        for (auto& letter: lower_arg1) {
            letter = std::tolower(letter);
        }
        if (Help.contains(lower_arg1)) {
            ostream << lower_arg1 << Help[lower_arg1] << L"\n";
        } else {
			throw ExceptionWithMessage(L"Справочная информация не найдена.");
        }
    } else {
        for (const auto& command: Help) {
            ostream << command.first << command.second << L"\n";
        }
    }
}

template <typename T> Matrix<T>& Handlers::GetMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::wstring& index) {
	try {
		size_t num = std::stoi(index) - 1; // Попытка найти матрицу по номеру идет первой. Если отвалится, то в обработчике
		if ((0 <= num) && (num < MatrixSet.size())) {										// stoi идет поиск по имени
			return MatrixSet[num]; // проверка на диапазон
		} else {
			throw ExceptionWithMessage(L"Такая матрица не найдена.");
		}
	} catch (const std::invalid_argument& InvalidArgument) {
		if (!index.empty()) {
			for (Matrix<T> &matrix: MatrixSet) { // Поиск по имени в массиве матриц
				if (matrix.name == index) {
					return matrix;        // Возвращается ссылка
				}
			}
			throw ExceptionWithMessage(L"Такая матрица не найдена.");
		} else {
			throw ExceptionWithMessage(L"Отсутствует имя матрицы.");	// Ошибка ненайденной матрицы
		}
	} catch (...) {
		throw;  // Неизвестная ошибка
	}
}

template <typename T> void Handlers::SetNameHandler(
		std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	for (auto& matrix: MatrixSet) {
		if (matrix.name != L"" && matrix.name == Arguments[2]) {
			throw ExceptionWithMessage(L"Матрица с таким именем уже существует.");
		}
	}
	auto& matrix = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	matrix.name = Arguments[2];
}

template <typename T> void Handlers::FillMatrixHandler(
		std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	unsigned char mode;
	if (!Arguments[2].empty()) {
		mode = Arguments[2][0];
	} else {
		throw ExceptionWithMessage(L"Нужен параметр режима заполнения");
	}
	switch (mode) {
		case 'i': {
			matrix.FillStorage(mode);
			break;
		}
		case 'r': {
			T left = std::stod(Arguments[3]);
			T right = std::stod(Arguments[4]);
			T value = 0.0;
			if (!Arguments[5].empty()) {
				value = std::stod(Arguments[5]);
			}
			matrix.FillStorage(mode, value, left, right);
			break;
		}
		case 'c': {
			if (!Arguments[3].empty()) {
				T value = std::stod(Arguments[3]);
				matrix.FillStorage(mode, value);
			} else {
				throw ExceptionWithMessage(L"Нет аргумента значения для заполнения в режиме \"c\" (constant).");
			}
			break;
		}
		default: {
			throw ExceptionWithMessage(L"Режим заполнения не найден.");
		}
	}
}

template <typename T> void Handlers::ResizeMatrixHandler(
		std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	uint32_t rows = std::stoul(Arguments[2]);
	uint32_t columns = std::stoul(Arguments[3]);
	if (rows == 0 || columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры матрицы.");
	}
	matrix.ResizeTo(rows, columns);
}

template <typename T> void Handlers::MatrixSelfMultiplicationHandler(
		std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	auto& matrix2 = Handlers::GetMatrixHandler(MatrixSet, Arguments[2]);
	if (matrix1.columns != matrix2.rows) {
		throw ExceptionWithMessage(L"Размеры матриц не позволяют их умножить.");
	} else if (matrix1.rows == 0 || matrix1.columns == 0 || matrix2.rows == 0 || matrix2.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры одной из матриц.");
	}
	matrix1.MultiplyWith(matrix2);
}

template <typename T> void Handlers::MatrixMultiplicationHandler(
		std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	auto& matrix2 = Handlers::GetMatrixHandler(MatrixSet, Arguments[2]);
	auto& matrix3 = Handlers::GetMatrixHandler(MatrixSet, Arguments[3]);
	if (matrix2.columns != matrix3.rows) {
		throw ExceptionWithMessage(L"Размеры матриц не позволяют их умножить.");
	} else if (matrix2.rows == 0 || matrix2.columns == 0 || matrix3.rows == 0 || matrix3.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры одной из матриц.");
	}
	auto name = matrix1.name;
	matrix1 = Matrix<T>(matrix2, matrix3);
	matrix1.name = name;
}

template <typename T> void Handlers::MultiplicationByMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	auto& matrix2 = Handlers::GetMatrixHandler(MatrixSet, Arguments[2]);
	if ((matrix1.rows != matrix2.rows) || (matrix1.columns != matrix2.columns)) {
		throw ExceptionWithMessage(L"Размеры матриц не совпадают.");
	}
	if (matrix1.rows == 0 || matrix1.columns == 0 || matrix2.rows == 0 || matrix2.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры одной из матрицы.");
	}
	matrix1.MultiplicationByMatrix(matrix2);
 }

template <typename T> void Handlers::AdditionByMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	auto& matrix2 = Handlers::GetMatrixHandler(MatrixSet, Arguments[2]);
	if ((matrix1.rows != matrix2.rows) || (matrix1.columns != matrix2.columns)) {
		throw ExceptionWithMessage(L"Размеры матриц не совпадают.");
	}
	if (matrix1.rows == 0 || matrix1.columns == 0 || matrix2.rows == 0 || matrix2.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры одной из матриц.");
	}
	matrix1.AdditionByMatrix(matrix2);
}

template <typename T> void Handlers::SubtractionByMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	auto& matrix2 = Handlers::GetMatrixHandler(MatrixSet, Arguments[2]);
	if ((matrix1.rows != matrix2.rows) || (matrix1.columns != matrix2.columns)) {
		throw ExceptionWithMessage(L"Размеры матриц не совпадают.");
	}
	if (matrix1.rows == 0 || matrix1.columns == 0 || matrix2.rows == 0 || matrix2.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры одной из матриц.");
	}
	matrix1.SubtractionByMatrix(matrix2);
}

template <typename T> void Handlers::DivisionByMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	auto& matrix2 = Handlers::GetMatrixHandler(MatrixSet, Arguments[2]);
	if ((matrix1.rows != matrix2.rows) || (matrix1.columns != matrix2.columns)) {
		throw ExceptionWithMessage(L"Размеры матриц не совпадают.");
	}
	if (matrix1.rows == 0 || matrix1.columns == 0 || matrix2.rows == 0 || matrix2.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры одной из матриц.");
	}
	matrix1.DivisionByMatrix(matrix2);
}

template <typename T> void Handlers::MultiplicationByScalarHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	double value = std::stod(Arguments[2]);
	if (matrix1.rows == 0 || matrix1.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры матрицы.");
	}
	matrix1.MultiplicationByScalar(value);
}

template <typename T> void Handlers::AdditionByScalarHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	double value = std::stod(Arguments[2]);
	if (matrix1.rows == 0 || matrix1.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры матрицы.");
	}
	matrix1.AdditionByScalar(value);
}

template <typename T> void Handlers::SubtractionByScalarHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	double value = std::stod(Arguments[2]);
	if (matrix1.rows == 0 || matrix1.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры матрицы.");
	}
	if (value == 0) {
		throw ExceptionWithMessage(L"Деление на ноль запрещено.");
	}
	matrix1.SubtractionByScalar(value);
}

template <typename T> void Handlers::DivisionByScalarHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	double value = std::stod(Arguments[2]);
	if (matrix1.rows == 0 || matrix1.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры матрицы.");
	}
	matrix1.DivisionByScalar(value);
}

template <typename T> void Handlers::SubmatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	auto& matrix2 = Handlers::GetMatrixHandler(MatrixSet, Arguments[2]);
	uint32_t row = std::stoul(Arguments[3]);
	uint32_t column = std::stoul(Arguments[4]);
	auto name = matrix1.name;
	matrix1 = matrix2.SubmatrixOf(row, column);
	matrix1.name = name;
}

template <typename T> void Handlers::TransposeHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	matrix.Transpose();
}


template <typename T> void Handlers::CopyMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix1 = Handlers::GetMatrixHandler(MatrixSet, Arguments[1]);
	auto& matrix2 = Handlers::GetMatrixHandler(MatrixSet, Arguments[2]);
	auto* new_storage = new std::vector<std::vector<T>>;
	auto name = matrix1.name;
	std::copy(matrix2._storage.begin(), matrix2._storage.end(), std::back_inserter(*new_storage));
	matrix1 = Matrix<T>(matrix2.rows, matrix2.columns, new_storage);
	matrix1.name = name;
}


std::basic_ifstream<wchar_t> Handlers::OpenIFileHandler(std::wstring& path) {
	std::filesystem::path filepath(path);
	std::basic_ifstream<wchar_t> file(filepath);
	if (!file.is_open()) {
		throw ExceptionWithMessage(L"Не найден файл для открытия.");
	}
	return file;
}

std::basic_ofstream<wchar_t> Handlers::OpenOFileHandler(std::wstring& path) {
	std::filesystem::path filepath(path);
	std::basic_ofstream<wchar_t> file (filepath);
	if (!file.is_open()) {
		throw ExceptionWithMessage(L"Запись в данную директорию запрещена или файл занят.");
	}
	return file;
}

template <typename T> void Handlers::LUTransformHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments) {
	auto& matrix = GetMatrixHandler(MatrixSet, Arguments[1]);
	if (matrix.rows != matrix.columns) {
		throw ExceptionWithMessage(L"Матрица не квадратная.");
	} else if (matrix.rows == 0 || matrix.columns == 0) {
		throw ExceptionWithMessage(L"Нулевые размеры матрицы.");
	} else {
		auto temp = matrix.lu_transform();
		GetMatrixHandler(MatrixSet, Arguments[2]) = *(temp[1]);
		GetMatrixHandler(MatrixSet, Arguments[3]) = *(temp[2]);
	}
}
