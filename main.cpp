#include <iostream>
#include "mitrix.h"
#include "service.h"


typedef double MATRIX_T;

int main() { // TODO: Вынести сервисные функции в service.cpp
	std::vector<Matrix<double>> matrix_set(10);
	for (auto &matrix: matrix_set) {
		matrix.resizeTo(10, 10);
		matrix.fillStorage('r', 0, -10, 10);
	}
	Handlers::ListHandler<double>(matrix_set);
}

// mitrix-cli