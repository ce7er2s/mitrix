//
// Created by reenie on 27.09.2021.
//

enum ERRORS {
	OK = 0,
	NEGATIVE_ARG = 1,
	FILE_NOT_FOUND = 2,
	ZERO_LENGTH = 3

};

template <typename T> int Handlers::ListHandler(
		const std::vector<Matrix<T>> &matrix_set,
		std::basic_ostream<wchar_t> &ostream) { // при манипуляциях с потоком нельзя использовать const
	ostream << std::setw(2) << L"#" << "|";
	ostream << std::setw(5) << L"ROWS" << "|";
	ostream << std::setw(8) << L"COLUMNS" << "|";
	ostream << std::setw(14) << L"STORAGE" << "|";
	ostream << std::setw(5) << L"NAME" << std::endl;
	ostream.width(28);
	ostream.fill('-');
	ostream << std::setw(2) << "" << "+";
	ostream << std::setw(5) << "" << "+";
	ostream << std::setw(8) << "" << "+";
	ostream << std::setw(14) << "" << "+";
	ostream << std::setw(16) << "" << std::endl;
	ostream.fill(' ');
	uint32_t i = 1;
	for (Matrix<T> matrix: matrix_set) {
		ostream << std::setw(2) << i++ << "|";
		ostream << std::setw(5) << matrix.rows << "|";
		ostream << std::setw(8) << matrix.columns << "|";
		ostream << std::setw(8) << sizeof(T) * matrix.rows * matrix.columns << std::setw(6) << L"bytes" << "|";
		if (!matrix.name.empty()) {
			ostream << std::left;
			ostream << std::setw(matrix.name.length() + 1);
			ostream << matrix.name << std::endl;
			ostream << std::right;
		}
		else {
			ostream << std::setw(5);
			ostream << L"NONE" << std::endl;
		}
	}
	return 0;
}

template <typename T> int Handlers::InputHandler(
		Matrix<T> &matrix,
		uint32_t rows,
		uint32_t columns,
		std::basic_istream<wchar_t>& istream
		) {
	if (rows < 0 || columns < 0) {
		return NEGATIVE_ARG;
	} else if (rows == 0 || columns == 0) {
		istream >> rows;
		istream >> columns;
		matrix.resizeTo(rows, columns);
	}
	for (uint32_t i = 0; i < rows; i++) {
		for (uint32_t j = 0; j < columns; j++) {
			istream >> matrix[i][j];
		}
	}
	return OK;

}

template <typename T> int Handlers::OutputHandler(
		Matrix<T> &matrix,
		std::basic_ostream<wchar_t>& ostream) {
	if (matrix.rows == 0 || matrix.columns == 0) {
		return ZERO_LENGTH;
	}
	ostream << matrix.rows << " " << matrix.columns << std::endl;
	for (uint32_t i = 0; i < matrix.rows; i++) {
		for (uint32_t j = 0; j < matrix.columns - 1; j++) {
			ostream << matrix[i][j] << " ";
		}
		ostream << matrix[i][matrix.columns-1] << std::endl;
	}
}

template <typename T> int Handlers::FormatOutputHandler(
		Matrix<T> &matrix,
		std::basic_ostream<wchar_t>& ostream,
		u_char precision) {
	if (matrix.rows == 0 || matrix.columns == 0) { // проверка на нулевую длину
		return ZERO_LENGTH;
	}

	size_t element_size = 0;  // Для правильной конвертации таблицы нужны размеры точные
	size_t row_numbers_size = std::to_string(matrix._storage.size()).length();
	size_t columns_numbers_size = std::to_string(matrix._storage[0].size()).length();

	for (std::vector<T> row: matrix._storage) {  // Поиск максимально длинного элемента
		for (T element: row) {
			element_size = std::max(element_size, std::to_wstring(element).length());
		}
		columns_numbers_size = std::max(columns_numbers_size, std::to_wstring(row.size()).length());
	}

	element_size = std::max(element_size, std::max(row_numbers_size, columns_numbers_size));  // Поиск наибольшего
	auto cell_size = static_cast<int32_t>(element_size);                                      // И его общая запись

	ostream << std::fixed << std::setprecision(std::min(precision,static_cast<u_char>(cell_size)));

	ostream << std::setw(cell_size) << "";
	for (uint32_t i = 1; i < matrix.rows; i++) {  // Отрисовка шапки с индексами:   1|  2|  3|
		ostream << "|" << std::setw(cell_size) << i;
	}
	ostream << std::setw(cell_size) << matrix.columns << std::endl;

	for (uint32_t i = 0; i < matrix.rows; i++) {  // Отрисовка полоски и рядов. Одна итерация -- полоска и ряд элементов

		ostream.fill('-');
		ostream << std::setw(cell_size) << "";  // Отрисовка полосок
		for (uint32_t j = 0; j < matrix.columns; j++) {
			ostream << '+' << std::setw(cell_size) << "";
		}
		ostream.fill(' ');
		ostream << std::endl;

		ostream << std::setw(cell_size) << i+1 << "|";  // Отрисовка элементов
		ostream << std::setw(cell_size) << matrix[i][0];
		for (uint32_t j = 1; j < matrix.columns; j++) {
			ostream << "|" << std::setw(cell_size) << matrix[i][j];
		}
		ostream << std::endl;
	}
	return 0;
}

template <typename T> int Handlers::MatrixMultiplicationHandler(
		const std::vector<Matrix<T>> &matrix1,
		const std::vector<Matrix<T>> &matrix2,
		std::vector<Matrix<T>> &matrix3,
		const std::ofstream& ofstream) {
	return 0;
}

template <typename T> int Handlers::MatrixSelfMultiplicationHandler(
		const std::vector<Matrix<T>> &matrix1,
		std::vector<Matrix<T>> &matrix2,
		const std::ofstream& ofstream) {
	return 0;
}

template <typename T> int Handlers::DeterminantHandler(
		const std::vector<Matrix<T>> &matrix,
		const std::ofstream& ofstream) {
	return 0;
}