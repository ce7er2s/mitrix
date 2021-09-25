#include <iostream>
#include <fstream>

#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	auto matrix = Matrix<int32_t>(10, 10); // Всякие проверки функциональности
	matrix.fillStorage('r', 0, 0, 9);
	matrix.printFormatTo();
	std::cout << std::endl << "Determinant is " << matrix.determinantOf() << "." << std::endl;
}

// mitrix-cli