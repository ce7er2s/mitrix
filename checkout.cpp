#include <iostream>
#include <random>
#include <vector>
#include "matrix.h"

int main () {
	std::vector<std::vector<double>> storage1 = {{2, 6, 7, 5, 3, 2},
												 {4, 3, 5, 6, 9, 9},
												 {8, 4, 1, 4, 4, 5},
												 {1, 5, 9, 5, 3, 2},
												 {4, 3, 2, 6, 0, 9},
												 {2, 7, 8, 9, 4, 5}};
	std::vector<std::vector<double>> storage2 = {{5},
												 {6},
												 {2},
												 {7},
												 {3},
												 {4}};
	Matrix<double> A(storage1);
	Matrix<double> B(storage2);

	auto solution = gauss_method<double>(A, B);
	for (auto& row: solution[0].storage) {
		for (auto& element: row) {
			std::cout << element << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\n" << solution[0].DeterminantOf() << "\n\n";

	for (auto& row: solution[1].storage) {
		for (auto& element: row) {
			std::cout << element << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\n";

	for (auto& row: solution[2].storage) {
		for (auto& element: row) {
			std::cout << element << " ";
		}
		std::cout << "\n";
	}

	auto LU = lu_transform(A);

	std::cout << "\n";

	for (auto& row: LU[0].storage) {
		for (auto& element: row) {
			std::cout << element << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\n";

	for (auto& row: LU[1].storage) {
		for (auto& element: row) {
			std::cout << element << " ";
		}
		std::cout << "\n";
	}

	LU[0].MultiplyWith(LU[1]);

	std::cout << "\n";

	for (auto& row: LU[0].storage) {
		for (auto& element: row) {
			std::cout << element << " ";
		}
		std::cout << "\n";
	}
}
