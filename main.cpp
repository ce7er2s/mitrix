#include <iostream>
#include <vector>

template <typename T> class Matrix {
	uint32_t columns = 0;
	uint32_t rows = 0;
public:
	std::vector<T, T> storage = std::vector<T, T>(rows, columns);
public:
	void operator[] (int n) {
		if (n >= 0)
			return this->storage[n];
		else
			return this->storage[this->rows-n];
	}
	explicit Matrix(uint32_t rows = 0, uint32_t columns = 0, std::vector<T, T>* _storage = NULL) {
		this->columns = columns;
		this->columns = rows;
		if (_storage == NULL)
			this->storage = std::vector<T, T>(rows, columns);
		else
			this->storage = *_storage;
	}
};

int main() {
	Matrix<uint32_t> matrix = new Matrix<uint32_t>(10, 10);
}
