#include <iostream>
#include <fstream>

#include "mitrix.h"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	Matrix<uint32_t> matrix1 = Matrix<uint32_t>(3, 4); // Всякие проверки функциональности
	Matrix<uint32_t> matrix2 = Matrix<uint32_t>(4, 5); // Всякие проверки функциональности
	Matrix<uint32_t> matrix3;
	std::ifstream ifile1 = std::ifstream("sample1.txt");
	std::ifstream ifile2 = std::ifstream("sample2.txt");
	matrix1.inputFrom(ifile1);
	matrix1.printTo();
	std::cout << std::endl;
	matrix2.inputFrom(ifile2);
	matrix2.printTo();
	std::cout << std::endl;
	matrix1 *= matrix2;
	matrix1.printTo();
}

// mitrix-cli