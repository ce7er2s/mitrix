//
// Created by reenie on 27.09.2021.
//

enum ERRORS {
	OK = 0,
	NEGATIVE_ARG = 1,
	FILE_NOT_FOUND = 2,
	ZERO_LENGTH = 3,
	NO_DETERMINANT = 4,
	MULTIPLICATION_IMPOSSIBLE = 5
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
			istream >> matrix._storage[i][j];
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
		columns_max_size = std::max(columns_max_size, std::to_wstring(row.size()).length());
	}

	element_max_size = std::max(element_max_size, columns_max_size);  // Поиск наибольшего
	auto cell_size = static_cast<int32_t>(element_max_size) + 1;                                  // И его общая запись
	auto rows_size = static_cast<u_char>(columns_max_size) + 1;
	// +1 для отступа.
	ostream << std::fixed << std::setprecision(precision);

	ostream << std::setw(rows_size) << "";
	for (uint32_t i = 0; i < matrix.columns; i++) {  // Отрисовка шапки с индексами: 1| 2| 3|
		ostream << " |" << std::setw(cell_size) << i+1;
	}
	ostream << std::endl;

	for (uint32_t i = 0; i < matrix.rows; i++) {  // Отрисовка полоски и рядов. Одна итерация -- полоска и ряд элементов

		ostream.fill('-'); // Отрисовка полосок
		ostream << std::setw(rows_size+1) << "";
		for (uint32_t j = 0; j < matrix.columns-1; j++)
			ostream << '+' << std::setw(cell_size+1) << "";
		ostream << "+" << std::setw(cell_size) << "";
		ostream.fill(' ');
		ostream << std::endl;

		ostream << std::setw(rows_size) << i+1 << " |";  // Отрисовка элементов
		ostream << std::setw(cell_size) << matrix[i][0];
		for (uint32_t j = 1; j < matrix.columns; j++) {
			ostream << " |" << std::setw(cell_size) << matrix[i][j];
		}
		ostream << std::endl;
	}
	return 0;
}

template <typename T> int Handlers::MatrixMultiplicationHandler(
		Matrix<T> &matrix1,
		Matrix<T> &matrix2,
		Matrix<T> &matrix3,
		std::basic_ostream<wchar_t>& ostream) {
	if (matrix1.columns != matrix2.rows) {
		return MULTIPLICATION_IMPOSSIBLE;
	}
	matrix3 = Matrix<T>(matrix1, matrix2);
	return 0;
}

template <typename T> int Handlers::MatrixSelfMultiplicationHandler(
		Matrix<T> &matrix1,
		Matrix<T> &matrix2,
		std::basic_ostream<wchar_t>& ostream) {
	if (matrix1.columns != matrix2.rows) {
		return MULTIPLICATION_IMPOSSIBLE;
	}
	matrix1.multiplyWith(matrix2);
	ostream << "Successful!";
	return 0;
}

template <typename T> int Handlers::DeterminantHandler(
		Matrix<T> &matrix,
		std::basic_ostream<wchar_t>& ostream) {
	if (matrix.rows != matrix.columns) {
		return NO_DETERMINANT;
	}
	ostream << "Determinant is " << matrix.determinantOf() << ".";
	return 0;
}