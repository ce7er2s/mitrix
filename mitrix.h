//
// Created by reenie on 29.09.2021.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>

template <typename T> class Matrix {
public:
	uint32_t columns = 0;
	uint32_t rows = 0;
	std::vector<std::vector<T>> _storage;
	std::string name;
public:
	T operator~ ();
	std::vector<T> operator[] (int32_t n);
	Matrix<T> operator* (Matrix<T>& _matrix);
	void operator*= (Matrix<T>& _matrix);

	Matrix (Matrix<T> _matrix_1, Matrix<T> _matrix_2);
	explicit Matrix(uint32_t _rows = 0, uint32_t _columns = 0, std::vector<std::vector<T>>* _storage = nullptr);
	Matrix<T> submatrixOf (uint32_t _row, uint32_t _column);

	void inputFrom(std::istream& _istream = std::cin, uint32_t _rows = 0, uint32_t _columns = 0);
	void printTo(std::ostream& _ostream = std::cout);
	void printFormatTo(std::ostream& _ostream = std::cout, char precision = 2);

	void resizeTo(uint32_t _rows, uint32_t _columns);
	void fillStorage(char mode = 'r', T value = 0, T left_border = 0, T right_border = 0);

	void multiplyWith(Matrix& _matrix);

	void multiplicationByMatrix(Matrix<T>& _matrix);
	void additionByMatrix(Matrix<T>& _matrix);
	void subtractionByMatrix(Matrix<T>& _matrix);
	void divisionByMatrix(Matrix<T>& _matrix);

	void multiplicationByScalar(T _value);
	void additionByScalar(T _value);
	void subtractionByScalar(T _value);
	void divisionByScalar(T _value);

	T determinantOf();
};

#include "mitrix.hpp"