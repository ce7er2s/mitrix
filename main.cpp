#include <iostream>
#include <fstream>

#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	Matrix<uint32_t> matrix = Matrix<uint32_t>(3, 4); // Всякие проверки функциональности
	std::ifstream ifile = std::ifstream("sample.txt");
	matrix.inputFrom(ifile);
	auto submatrix = matrix.submatrixOf(1, 2);
	matrix.printTo();
	std::cout << std::endl;
	submatrix.printTo();
}

// mitrix-cli