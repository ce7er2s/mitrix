//
// Created by reenie on 22.09.2021.
//

#include <iostream>
#include <vector>

template <typename T> class Matrix {
	uint32_t columns = 0;
	uint32_t rows = 0;
	std::vector<std::vector<T>> _storage;
public:
	std::vector<T> operator[] (int32_t n) {
		return this->_storage[n];	// TODO: Реализация доступа к приватному _storage по отрицательному индексу aka Python-style
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

	void inputFrom(std::istream& _istream = std::cin, uint32_t _rows = 0, uint32_t _columns = 0) {
		if (_rows != 0 || _columns != 0) {
			if (_rows == 0)        // _stream это ссылка на поток ввода (std::ifstream легко кастуется в std::istream)
				_rows = this->rows;        // Если хотя бы один размер матрицы ненулевой, ее размеры меняются
			if (_columns == 0)
				_columns = this->columns;
			this->resizeTo(_rows, _columns); // Изменение размеров матрицы.
		} else {
			_rows = this->rows; // Вариант без переразметки.
			_columns = this->columns;
		}
		for (uint32_t i = 0; i < _rows; i++) // Чтение из переданного потока. Почему-то падает с 139 (SEGFAULT)
			for (uint32_t j = 0; j < _columns; j++) // UPD: Балда, там был инициализирован пустой массив
				_istream >> this->_storage[i][j];									// с ненулевым размером.
	}

	void printTo(std::ostream& _ostream = std::cout) {
		for (uint32_t i = 0; i < this->rows; i++) {
			for (uint32_t j = 0; j < this->columns; j++) {
				_ostream << this->_storage[i][j];
				_ostream << " "; // TODO: сделать разделитель отдельным атрибутом
			}
			_ostream << std::endl;
		}
	}

	void resizeTo(uint32_t _rows, uint32_t _columns) {
		for (uint32_t i = 0; i < this->rows; i++) {
			for (uint32_t j = 0; j < this->columns; j++)
				this->_storage[i].resize(_columns); // Изменение размера внутренних массивов _storage
		}
		this->_storage.resize(_rows); // Изменение размера самого _storage
		this->rows = _rows; // Запись размеров
		this->columns = _columns;
	}
	Matrix<T> submatrixOf (uint32_t _row, uint32_t _column) {
		Matrix<T> temp = Matrix<T>((this->rows)-1, (this->columns)-1);
		uint32_t k = 0; // Номер ряда новой матрицы
		for (uint32_t i = 0; i < this->rows; i++) {
			uint32_t l = 0; // Номер столбца новой матрицы
			for (uint32_t j = 0; j < this->columns; j++) {
				if (i != _row and j != _column) // Запись только если координаты не совпадают с вычеркнутыми полями.
					temp._storage[k][l++] = this->_storage[i][j]; // Запись в temp с инкреминированием l
			}
			if (i != _row) {
				k += 1;
			}
		}
		return temp;
	}
	void multiplyWith(Matrix& _matrix) {
		std::vector<std::vector<T>> temp;
		uint32_t size = _matrix.rows;
		uint32_t temp_rows = this->rows;
		uint32_t temp_columns = _matrix.columns;
		temp.resize(temp_rows);
		for (uint32_t i = 0; i < size; i++)
			temp[i].resize(temp_columns);
		for (uint32_t i = 0; i < temp_rows; i++) {
			for (uint32_t j = 0; j < temp_columns; j++) {
				for (uint32_t k = 0; k < size; k++)
					temp[i][j] += this->_storage[i][k] * _matrix._storage[k][j];
			}
		}
		this->_storage = temp;
		this->columns = temp_columns;
	}
	T determinantOf() {
		if (this->rows == this->columns) {
			if (this->rows == 2) {
				return this->_storage[0][0]*this->_storage[1][1] - this->_storage[0][1]*this->_storage[1][0];
			} else {
				T determinant = 0;
				for (uint32_t i = 0; i < this->rows; i++) {
					for (uint32_t j = 0; j < this->columns; j++) {
						if (i+j % 2 == 0) {
							determinant += (this->_storage[i][j] * this->submatrixOf(i,j).determinantOf());
						} else {
							determinant -= (this->_storage[i][j] * this->submatrixOf(i,j).determinantOf());
						}
					}
				}
				return determinant;
			}
		} else {
			return;
		}
	}
};
