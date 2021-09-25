#include <iostream>
#include <fstream>

#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	auto matrix = Matrix<double>(10, 10); // Всякие проверки функциональности
	matrix.fillStorage('i');
	matrix.printFormatTo();
	//std::cout << std::endl << "Determinant is " << matrix.determinantOf() << "." << std::endl;
	std::cout << matrix[0][0] << std::endl;
	std::cout << std::to_string(matrix[0][0]) << std::endl;
	std::cout << std::to_string(matrix[0][0]).length() << std::endl;
}

// mitrix-cli