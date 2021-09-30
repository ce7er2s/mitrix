//
// Created by reenie on 27.09.2021.
//

template <typename T> int Handlers::ListHandler(
		const std::vector<Matrix<T>> &matrix_set,
		std::ostream &ostream // при манипуляциях с потоком нельзя использовать const
		) {
	ostream << std::setw(2) << "№" << "|";
	ostream << std::setw(5) << "ROWS" << "|";
	ostream << std::setw(8) << "COLUMNS" << "|";
	ostream << std::setw(8) << "STORAGE" << "|";
	ostream << std::setw(5) << "NAME" << std::endl;
	ostream.width(28);
	ostream.fill('=');
	for (uint32_t i = 0; i < matrix_set.size(); i++) {
		ostream << std::setw(2) << i << "|";
		ostream << std::setw(5) << matrix_set[i].rows << "|";
		ostream << std::setw(8) << matrix_set[i].columns << "|";
		ostream << std::setw(8) << sizeof(matrix_set[i]._storage) << "|";
		ostream << std::setw(5) << matrix_set[i].name << std::endl;
	}
}

template <typename T> int Handlers::InputHandler(
		Matrix<T> &matrix,
		const std::ifstream& ifstream
		) {
	;
}

template <typename T> int Handlers::OutputHandler(
		const Matrix<T> &matrix,
		const std::ofstream& ofstream
		) {
	;
}

template <typename T> int Handlers::FormatOutputHandler(
		const std::vector<Matrix<T>> &matrix,
		const std::ofstream& ofstream
		) {
	;
}

template <typename T> int Handlers::MatrixMultiplicationHandler(
		const std::vector<Matrix<T>> &matrix1,
		const std::vector<Matrix<T>> &matrix2,
		std::vector<Matrix<T>> &matrix3,
		const std::ofstream& ofstream
		) {
	;
}

template <typename T> int Handlers::MatrixSelfMultiplicationHandler(
		const std::vector<Matrix<T>> &matrix1,
		std::vector<Matrix<T>> &matrix2,
		const std::ofstream& ofstream
		) {
	;
}

template <typename T> int Handlers::DeterminantHandler(
		const std::vector<Matrix<T>> &matrix,
		const std::ofstream& ofstream
		) {
	;
}