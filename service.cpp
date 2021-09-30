//
// Created by reenie on 27.09.2021.
//

template <typename T> int Handlers::ListHandler(
		const std::vector<Matrix<T>> &matrix_set,
		std::ostream &ostream ) { // при манипуляциях с потоком нельзя использовать const
	ostream << std::setw(2) << "#" << "|";
	ostream << std::setw(5) << "ROWS" << "|";
	ostream << std::setw(8) << "COLUMNS" << "|";
	ostream << std::setw(14) << "STORAGE" << "|";
	ostream << std::setw(5) << "NAME" << std::endl;
	ostream.width(28);
	ostream.fill('-');
	ostream << std::setw(2) << "" << "+";
	ostream << std::setw(5) << "" << "+";
	ostream << std::setw(8) << "" << "+";
	ostream << std::setw(14) << "" << "+";
	ostream << std::setw(5) << "" << std::endl;
	ostream.fill(' ');
	uint32_t i = 1;
	for (Matrix<T> matrix: matrix_set) {
		ostream << std::setw(2) << i++ << "|";
		ostream << std::setw(5) << matrix.rows << "|";
		ostream << std::setw(8) << matrix.columns << "|";
		ostream << std::setw(8) << sizeof(T) * matrix.rows * matrix.columns << std::setw(6) << "bytes" << "|";
		ostream << " " << std::left;
		if (matrix.name != "") {
			ostream << std::setw(matrix.name.length());
			ostream << matrix.name << std::endl;
		}
		else {
			ostream << std::setw(4);
			ostream << "NONE" << std::endl;
		}
		ostream << std::right;
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