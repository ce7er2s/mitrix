#include <iostream>
#include <fstream>
#include <ctime>

#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	auto matrix = Matrix<int64_t>(11, 2); // Всякие проверки функциональности
	matrix.fillStorage('r', 0, 0, 100);
	matrix.printFormatTo(std::cout, 2);
	//std::cout << std::endl << "Determinant is " << matrix.determinantOf() << "." << std::endl;
	std::cout << matrix[0][0] << std::endl;
	std::cout << std::to_string(matrix[0][0]) << std::endl;
	std::cout << std::to_string(matrix[0][0]).length() << std::endl;
}

// mitrix-cli