#include <iostream>
#include "mitrix.h"
#include "service.h"


typedef double MATRIX_T;

int main() { // TODO: Вынести сервисные функции в service.cpp
	setlocale(LC_CTYPE, "");
	std::wfstream output("output.txt");
	std::vector<Matrix<MATRIX_T>> matrix_set(10);
	for (Matrix<MATRIX_T> &matrix: matrix_set) {
		matrix.resizeTo(10, 10);
		matrix.fillStorage('r', 0, -11, 1);
	}
	Handlers::InputHandler<MATRIX_T>(matrix_set[0]);
	Handlers::FormatOutputHandler<MATRIX_T>(matrix_set[0]);
	Handlers::ListHandler<MATRIX_T>(matrix_set, std::wcout);
}

// mitrix-cli