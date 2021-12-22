#include <iostream>
#include <random>
#include <vector>
#include "matrix.h"

int main () {
	std::vector<std::vector<double>> storage1 = {{2, 1}, {2, 3}};
	std::vector<std::vector<double>> storage2 = {{7, 8}, {9, 5}};
	Matrix<double> matrix1(2, 2, &storage1);
	Matrix<double> matrix2(2, 2, &storage2);
	std::cout << &matrix1 << " " << &matrix2 << "\n";
	matrix1 = std::move(matrix2);
	std::cout << &matrix1 << "\n";
}
