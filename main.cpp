#include <iostream>
#include <vector>

template <typename T> class Matrix {

	uint32_t columns = 0;
	uint32_t rows = 0;
	std::vector<std::vector<T>> _storage;
public:
	void operator[] (int32_t n) {  // Реализация доступа по отрицательному индексу aka Python-style
		if (n >= 0)
			return this->_storage[n];
		else
			return this->_storage[(this->rows)-n];
	}
	explicit Matrix(uint32_t rows = 0, uint32_t columns = 0, std::vector<std::vector<T>>* _storage = nullptr) {
		this->columns = columns;
		this->rows = rows;
		if (_storage == nullptr) {  // Запись в хранилище
			this->_storage = std::vector<std::vector<T>>(rows);
			for (size_t i = 0; i < rows; i++)
				this->_storage[i].resize(this->columns);
		} else {
			this->_storage = *_storage;
		}
	}
};

int main() {
	auto* _storage = new std::vector<std::vector<uint32_t>>;
	Matrix<uint32_t> matrix = Matrix<uint32_t>(2, 3, _storage);
	;
}
