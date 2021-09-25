#include <iostream>
#include <fstream>

#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	auto matrix = Matrix<double>(3, 1); // Всякие проверки функциональности
	matrix.fillStorage('r', 0, -100, 100);
	matrix.printFormatTo(std::cout, 2);
	//std::cout << std::endl << "Determinant is " << matrix.determinantOf() << "." << std::endl;
	std::cout << matrix[0][0] << std::endl;
	std::cout << std::to_string(matrix[0][0]) << std::endl;
	std::cout << std::to_string(matrix[0][0]).length() << std::endl;
}

// mitrix-cli