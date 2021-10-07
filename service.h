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
			std::vector<Matrix<T>>& MatrixSet,
			std::vector<std::wstring>& Arguments,
			std::basic_ostream<wchar_t>& ostream  // при манипуляциях с потоком нельзя использовать const
			);

	template <typename T> void InputHandler(
			std::vector<Matrix<T>>& MatrixSet,
			std::vector<std::wstring>& Arguments,
			std::basic_istream<wchar_t>& istream
			);

	template <typename T> void OutputHandler(
			std::vector<Matrix<T>>& MatrixSet,
			std::vector<std::wstring>& Arguments,
			std::basic_ostream<wchar_t>& ostream
			);

	template <typename T> void FormatOutputHandler(
			std::vector<Matrix<T>>& MatrixSet,
			std::vector<std::wstring>& Arguments,
			std::basic_ostream<wchar_t>& ostream
			);

	template <typename T> void MatrixMultiplicationHandler(
			std::vector<Matrix<T>>& MatrixSet,
			std::vector<std::wstring>& Arguments,
			std::basic_ostream<wchar_t>& ostream
			);

	template <typename T> void MatrixSelfMultiplicationHandler(
			std::vector<Matrix<T>>& MatrixSet,
			std::vector<std::wstring>& Arguments,
			std::basic_ostream<wchar_t>& ostream = std::wcout
			);

	template <typename T> void DeterminantHandler(
			std::vector<Matrix<T>>& MatrixSet,
			std::vector<std::wstring>& Arguments,
			std::basic_ostream<wchar_t>& ostream = std::wcout
			);

	template <typename T> Matrix<T>& GetMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::wstring& index);

	std::basic_ifstream<wchar_t> OpenIFileHandler(std::wstring& path);

	std::basic_ofstream<wchar_t> OpenOFileHandler(std::wstring& path);
}

#include "service.cpp"