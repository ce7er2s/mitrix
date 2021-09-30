//
// Created by reenie on 27.09.2021.
//

template <typename T> int Handlers::ListHandler(
		const std::vector<Matrix<T>> &matrix_set,
		std::basic_ostream<wchar_t> &ostream) { // при манипуляциях с потоком нельзя использовать const
	ostream << std::setw(2) << L"#" << "|";
	ostream << std::setw(6) << L"РЯДОВ" << "|";
	ostream << std::setw(9) << L"СТОЛБЦОВ" << "|";
	ostream << std::setw(14) << L"ЗАНИМАЕТ" << "|";
	ostream << std::setw(4) << L"ИМЯ" << std::endl;
	ostream.width(28);
	ostream.fill('-');
	ostream << std::setw(2) << "" << "+";
	ostream << std::setw(6) << "" << "+";
	ostream << std::setw(9) << "" << "+";
	ostream << std::setw(14) << "" << "+";
	ostream << std::setw(16) << "" << std::endl;
	ostream.fill(' ');
	uint32_t i = 1;
	for (Matrix<T> matrix: matrix_set) {
		ostream << std::setw(2) << i++ << "|";
		ostream << std::setw(6) << matrix.rows << "|";
		ostream << std::setw(9) << matrix.columns << "|";
		ostream << std::setw(9) << sizeof(T) * matrix.rows * matrix.columns << std::setw(5) << L"байт" << "|";
		if (!matrix.name.empty()) {
			ostream << std::left;
			ostream << std::setw(matrix.name.length() + 1);
			ostream << matrix.name << std::endl;
			ostream << std::right;
		}
		else {
			ostream << std::setw(4);
			ostream << L"НЕТ" << std::endl;
		}
	}
	return 0;
}

template <typename T> int Handlers::InputHandler(
		Matrix<T> &matrix,
		const std::ifstream& ifstream) {
	return 0;
}

template <typename T> int Handlers::OutputHandler(
		const Matrix<T> &matrix,
		const std::ofstream& ofstream) {
	return 0;
}

template <typename T> int Handlers::FormatOutputHandler(
		const std::vector<Matrix<T>> &matrix,
		const std::ofstream& ofstream) {
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