//
// Created by reenie on 30.09.2021.
//

#ifndef SERVICE_H
#define SERVICE_H

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


	template <typename T> void DeterminantHandler(
			std::vector<Matrix<T>>& MatrixSet,
			std::vector<std::wstring>& Arguments,
			std::basic_ostream<wchar_t>& ostream = std::wcout
			);

    void HelpHandler(std::vector<std::wstring>& Arguments,
                     std::map<std::wstring, std::wstring> Help,
                     std::basic_ostream<wchar_t>& ostream
                     );

	template <typename T> void SetNameHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	template <typename T> void FillMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	template <typename T> void ResizeMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	template <typename T> void MatrixMultiplicationHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	template <typename T> void MatrixSelfMultiplicationHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);
	
	template <typename T> void MultiplicationByMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);
	
	template <typename T> void AdditionByMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);
	
	template <typename T> void SubtractionByMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);
	
	template <typename T> void DivisionByMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	template <typename T> void MultiplicationByScalarHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);
	
	template <typename T> void AdditionByScalarHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);
	
	template <typename T> void SubtractionByScalarHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);
	
	template <typename T> void DivisionByScalarHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	template <typename T> Matrix<T>& GetMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::wstring& index);

	template <typename T> void SubmatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	template <typename T> void CopyMatrixHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	template <typename T> void TransposeHandler(std::vector<Matrix<T>>& MatrixSet, std::vector<std::wstring>& Arguments);

	std::basic_ifstream<wchar_t> OpenIFileHandler(std::wstring& path);

	std::basic_ofstream<wchar_t> OpenOFileHandler(std::wstring& path);


}

#include "handlers.cpp"

#endif //SERVICE_H