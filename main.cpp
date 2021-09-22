#include <iostream>
#include <vector>

template <typename T> class Matrix {
	uint32_t columns = 0;
	uint32_t rows = 0;
private:
	std::vector<std::vector<T>> _storage;
public:
	void operator[] (int32_t n) {  // Реализация доступа к приватному _storage по отрицательному индексу aka Python-style
		if (n >= 0)
			return this->_storage[n];
		else
			return this->_storage[(this->rows)-n];
	}
	explicit Matrix(uint32_t _rows = 0, uint32_t _columns = 0, std::vector<std::vector<T>>* _storage = nullptr) {
		this->columns = _columns;
		this->rows = _rows;
		if (_storage == nullptr) {  // Запись в хранилище
			this->_storage = std::vector<std::vector<T>>(rows); // иницализация масссива _storage
			for (size_t i = 0; i < rows; i++)
				this->_storage[i].resize(this->columns);
		} else {
			this->_storage = *_storage; // запись при наличии готового массива. Полезно при копировании матрицы.
		}								// здесь добавить функцию get_storage c копированием массива для этого случая.
	}
	void inputFrom(std::istream& _stream = std::cin, uint32_t _rows = 0, uint32_t _columns = 0) {
		if (_rows != 0 || _columns != 0) {
			if (_rows == 0)        // _stream это ссылка на поток ввода (std::ifstream легко кастуется в std::istream)
				_rows = this->rows;        // Если хотя бы один размер матрицы ненулевой, ее размеры меняются
			if (_columns == 0)
				_columns = this->columns;
			this->resizeTo(_rows, _columns); // Изменение размеров матрицы.
		} else {
			_rows = this->rows;
			_columns = this->columns;
		}
		for (uint32_t i = 0; i < _rows; i++)
			for (uint32_t j = 0; j < _columns; j++)
				_stream >> this->_storage[i][j];
	}

	void resizeTo(uint32_t _rows, uint32_t _columns) {
		for (uint32_t i = 0; i < this->rows; i++) {
			for (uint32_t j = 0; j < this->columns; j++)
				this->_storage[i].resize(_columns);
		}
		this->_storage.resize(_rows);
		this->rows = _rows;
		this->columns = _columns;
	}
};

int main() {
	auto* _storage = new std::vector<std::vector<uint32_t>>;
	Matrix<uint32_t> matrix = Matrix<uint32_t>(2, 3, _storage);
	matrix.inputFrom(std::cin);
}

// mitrix-cli