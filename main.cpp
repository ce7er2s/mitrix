#include <iostream>
#include <fstream>

#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	auto matrix1 = Matrix<int64_t>(4, 4); // Всякие проверки функциональности
	auto matrix2 = Matrix<float>(5, 13); // Всякие проверки функциональности
	matrix2.fillStorage('c', 1);
	matrix2.printTo();
}

// mitrix-cli