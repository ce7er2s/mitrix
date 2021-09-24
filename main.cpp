#include <iostream>
#include <fstream>

#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	auto matrix1 = Matrix<int64_t>(4, 4); // Всякие проверки функциональности
	auto matrix2 = Matrix<int64_t>(5, 5); // Всякие проверки функциональности
	Matrix<uint32_t> matrix3;
	auto ifile1 = std::ifstream("sample1.txt");
	auto ifile2 = std::ifstream("sample2.txt");
	matrix1.inputFrom(ifile1);
	matrix2.inputFrom(ifile2);
	matrix1.printTo();
	std::cout << ~matrix1 << std::endl << std::endl;
	matrix2.printTo();
	std::cout << ~matrix2 << std::endl << std::endl;
}

// mitrix-cli