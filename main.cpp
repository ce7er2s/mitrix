#include <iostream>
#include "mitrix.h"
#include "service.h"


typedef double MATRIX_T;

int main() { // TODO: Вынести сервисные функции в service.cpp
	std::ofstream output("output.txt");
	std::vector<Matrix<MATRIX_T>> matrix_set(10);
	for (Matrix<MATRIX_T> &matrix: matrix_set) {
		matrix.resizeTo(10, 10);
		matrix.fillStorage('r', 0, -10, 10);
	}
	Handlers::ListHandler<MATRIX_T>(matrix_set, output);
}

// mitrix-cli