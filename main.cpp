#include <iostream>
#include <vector>

template <typename T> class Matrix {
	uint32_t columns = 0;
	uint32_t rows = 0;
	std::vector<std::vector<T>> storage;
public:
	void operator[] (int32_t n) {
		if (n >= 0)
			return this->storage[n];
		else
			return this->storage[(this->rows)-n];
	}
	explicit Matrix(uint32_t rows = 0, uint32_t columns = 0, std::vector<std::vector<T>>* _storage = nullptr) {
		this->columns = columns;
		this->columns = rows;
		if (_storage == nullptr) {
			this->storage = std::vector<std::vector<T>>(rows);
			for (size_t i = 0; i < rows; i++)
				this->storage[i].resize(this->columns);
		} else {
			this->storage = *_storage;
		}
	}
};

int main() {
	Matrix<uint32_t> matrix = Matrix<uint32_t>(10, 10);
}
