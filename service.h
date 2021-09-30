//
// Created by reenie on 30.09.2021.
//

#ifndef UNTITLED_SERVICE_H
#define UNTITLED_SERVICE_H

#endif //UNTITLED_SERVICE_H

#include <iostream>
#include <fstream>
#include <iomanip>

namespace Handlers {
	template <typename T> int ListHandler(
			const std::vector<Matrix<T>> &matrix_set,
			std::ostream &ostream = std::cout  // при манипуляциях с потоком нельзя использовать const
			);

	template <typename T> int InputHandler(
			Matrix<T> &matrix,
			const std::ifstream& ifstream
			);

	template <typename T> int OutputHandler(
			const Matrix<T> &matrix,
			const std::ofstream& ofstream
			);

	template <typename T> int FormatOutputHandler(
			const std::vector<Matrix<T>> &matrix,
			const std::ofstream& ofstream
			);

	template <typename T> int MatrixMultiplicationHandler(
			const std::vector<Matrix<T>> &matrix1,
			const std::vector<Matrix<T>> &matrix2,
			std::vector<Matrix<T>> &matrix3,
			const std::ofstream& ofstream
			);

	template <typename T> int MatrixSelfMultiplicationHandler(
			const std::vector<Matrix<T>> &matrix1,
			std::vector<Matrix<T>> &matrix2,
			const std::ofstream& ofstream
			);

	template <typename T> int DeterminantHandler(
			const std::vector<Matrix<T>> &matrix,
			const std::ofstream& ofstream
			);
}

#include "service.cpp"