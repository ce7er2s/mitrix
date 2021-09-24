//
// Created by reenie on 22.09.2021.
//

#include <iostream>
#include <vector>
#include <random>

template <typename T> class Matrix {
	uint32_t columns = 0;
	uint32_t rows = 0;
	std::vector<std::vector<T>> _storage;
public:
	T operator~ () {
		return this->determinantOf();
	}
	std::vector<T> operator[] (int32_t n) {
		return this->_storage[n];	// TODO: Реализация доступа к приватному _storage по отрицательному индексу aka Python-style
	}
	Matrix<T> operator* (Matrix& _matrix_2) { // Надо проверить как я в принципе передаю параметры.
		return Matrix<T>(*this, _matrix_2);	//  Плохо работает.
	}
	void operator*= (Matrix& _matrix) {
		this->multiplyWith(_matrix);
	}
	Matrix (Matrix _matrix_1, Matrix _matrix_2) { // умножение на основе метода multiplyWith
		this->rows = _matrix_1.rows;		// Плохо работает.
		this->columns = _matrix_2.columns;
		this->_storage.swap(_matrix_1._storage);
		this->multiplyWith(_matrix_2);
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
		for (uint32_t i = 0; i < temp_rows; i++) { // Ниибаца тут тройной цикл.
			for (uint32_t j = 0; j < temp_columns; j++) {
				for (uint32_t k = 0; k < size; k++)
					temp[i][j] += this->_storage[i][k] * _matrix._storage[k][j];
			}
		}
		this->_storage.swap(temp);
		this->columns = temp_columns; // Сам
	}
	T determinantOf() {
		if (this->rows == this->columns) { // Вычисляется только для кавдратной матрицы
			if (this->rows == 2) { // Конечный случай для 2x2 матрицы
				return (this->_storage[0][0]*this->_storage[1][1]) - (this->_storage[0][1]*this->_storage[1][0]);
			} else if (this->rows == 1) {
				return this->_storage[0][0]; // Отдельный случай.
			} else {
				T determinant = 0;
				for (uint32_t j = 0; j < this->columns; j++) {
					 // Зачем умножать, если это ноль?
					if (this->_storage[0][j] == 0) {
						continue;
					}
					Matrix<T> submatrix = this->submatrixOf(0, j);
					if (j % 2 == 0) { // Миноры одной строчки.
						determinant += this->_storage[0][j] * submatrix.determinantOf();
					} else { // Для "четных" элементов произведение элемента и детерминанта его минора складывается и наоборот.
						determinant -= this->_storage[0][j] * submatrix.determinantOf();
					}
				}
				return determinant;
			}
		}
		else {
			throw std::exception(); // Я не знаю, что тут возвращать. Нужен какой-то NONE; сейчас тут throw
		}
	}
	void fillStorage(char mode = 'r', T value = 0, T left_border = 0, T right_border = 10) {
		switch (mode) {
			  case 'r': {
				  if (value == 0) {
					  std::random_device rd;
					  static std::mt19937 source(rd());
				  } else {
					  static std::mt19937 source(value);
				  }
				  if (std::is_floating_point(value)) {
					  std::uniform_real_distribution<T> distributor(left_border, right_border);
				  } else {
					  std::mt19937 source(value);
					  std::uniform_int_distribution<T> distributor(left_border, right_border);
				  }
				  for (uint32_t i = 0; i < this->rows; i++) {
					  for (uint32_t j = 0; j < this; j++) {
						  this->_storage[i][j] = distributor(source);
					  }
				}
			} case 'i': {
				uint32_t i = 0;
				while (i < this->rows && i < this->columns) {
					this->_storage[i][i] = 1;
					i++;
				}
			} case 'c': {
				for (uint32_t i = 0; i < this->rows; i++) {
					for (uint32_t j = 0; j < this->columns; j++) {
						this->_storage[i][j] = left_border;
					}
				}
			} default: {
				std::cerr << "fill mode " << mode << " is not specified" << std::endl;
			}
		}
	}
};
