#include <iostream>

#include "mitrix.cpp"  // TODO: разнести объявление и имплементацию Matrix

int main() {
	Matrix<uint32_t> matrix = Matrix<uint32_t>(2, 3);
	std::ifstream ifile = std::ifstream("sample_out.txt");
	matrix.inputFrom(ifile);
	matrix.printTo();
	std::cout << matrix[1][1];
}

// mitrix-cli