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
	template <typename T> void ListHandler(
			const std::vector<Matrix<T>> &matrix_set,
			std::basic_ostream<wchar_t> &ostream = std::wcout  // при манипуляциях с потоком нельзя использовать const
			);

	template <typename T> void InputHandler(
			Matrix<T> &matrix,
			uint32_t rows = 0,
			uint32_t columns = 0,
			std::basic_istream<wchar_t>& istream = std::wcin
			);

	template <typename T> void OutputHandler(
			Matrix<T> &matrix,
			std::basic_ostream<wchar_t>& ostream = std::wcout
			);

	template <typename T> void FormatOutputHandler(
			Matrix<T> &matrix,
			std::basic_ostream<wchar_t>& ostream = std::wcout,
			u_char precision = 4
			);

	template <typename T> void MatrixMultiplicationHandler(
			Matrix<T> &matrix1,
			Matrix<T> &matrix2,
			Matrix<T> &matrix3,
			std::basic_ostream<wchar_t>& ostream = std::wcout
			);

	template <typename T> void MatrixSelfMultiplicationHandler(
			Matrix<T> &matrix1,
			Matrix<T> &matrix2,
			std::basic_ostream<wchar_t>& ostream = std::wcout
			);

	template <typename T> void DeterminantHandler(
			Matrix<T> &matrix,
			std::basic_ostream<wchar_t>& ostream = std::wcout
			);
}

#include "service.cpp"