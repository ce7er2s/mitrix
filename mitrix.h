//
// Created by reenie on 29.09.2021.
//

#ifndef MITRIX_H
#define MITRIX_H


template <typename T> class Matrix {
public:
	uint32_t columns = 0;
	uint32_t rows = 0;
	std::vector<std::vector<T>> _storage;
	std::wstring name;
public:
	std::vector<T>& operator[] (int32_t n);

	explicit Matrix (Matrix<T> _matrix_1, Matrix<T> _matrix_2);
	explicit Matrix(uint32_t _rows = 0, uint32_t _columns = 0, std::vector<std::vector<T>>* _storage = nullptr);
	Matrix<T> SubmatrixOf (uint32_t _row, uint32_t _column);

	void ResizeTo(uint32_t _rows, uint32_t _columns);
	void FillStorage(unsigned char mode = 'r', T value = 0, T left_border = 0, T right_border = 0);

	void MultiplyWith(Matrix& _matrix);
	void MultiplicationByMatrix(Matrix<T>& _matrix);
	void AdditionByMatrix(Matrix<T>& _matrix);
	void SubtractionByMatrix(Matrix<T>& _matrix);
	void DivisionByMatrix(Matrix<T>& _matrix);

	void MultiplicationByScalar(T _value);
	void AdditionByScalar(T _value);
	void SubtractionByScalar(T _value);
	void DivisionByScalar(T _value);

	T DeterminantOf();
};

#include "mitrix.hpp"

#endif // MITRIX_H