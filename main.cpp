#include <iostream>
#include <vector>

template <typename T> class Matrix {
	uint32_t columns = 0;
	uint32_t rows = 0;
	std::vector<T, T> storage;
public:
	void operator[] (int n) {
		return this->storage[n];
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
	Matrix<uint32_t> matrix = Matrix<uint32_t>(10, 10);
}
