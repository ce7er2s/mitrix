//
// Created by reenie on 22.09.2021.
//


template <typename T> std::vector<T>& Matrix<T>::operator[] (int32_t n) {
	return this->_storage[n];	// TODO: Реализация доступа к приватному _storage по отрицательному индексу aka Python-style
}

template <typename T> Matrix<T>::Matrix(Matrix<T> _matrix_1, Matrix _matrix_2) { // умножение на основе метода multiplyWith
	this->rows = _matrix_1.rows;
	this->columns = _matrix_1.columns;
	this->_storage.swap(_matrix_1._storage); // Копирование матрицы и её атрибутов.
	this->MultiplyWith(_matrix_2);			// Умножение на вторую матрицу
}

template <typename T> Matrix<T>::Matrix(uint32_t _rows, uint32_t _columns, std::vector<std::vector<T>>* _storage) {
	this->columns = _columns;
	this->rows = _rows;
	if (_storage == nullptr) {  // Запись в хранилище
		this->_storage = std::vector<std::vector<T>>(rows); // иницализация масссива _storage
		for (size_t i = 0; i < rows; i++)
			this->_storage[i].resize(this->columns);
	} else {
		this->_storage.swap(*_storage); // запись при наличии готового массива. Полезно при копировании матрицы.
	}								// здесь добавить функцию get_storage c копированием массива для этого случая.
}

template <typename T> Matrix<T>::~Matrix () {
	for (auto& row: this->_storage) {
		row.clear();
		row.shrink_to_fit();
	}
	this->_storage.clear();
	this->_storage.shrink_to_fit();

/*	delete &(this->_storage);
	delete &(this->rows);
	delete &(this->columns);
	delete &(this->name); */
}

template <typename T> void Matrix<T>::ResizeTo(uint32_t _rows, uint32_t _columns) {
	this->rows = _rows; // Запись размеров
	this->columns = _columns;
	this->_storage.resize(_rows); // Изменение размера самого _storage
	for (uint32_t i = 0; i < this->rows; i++) {
		this->_storage[i].resize(_columns); // Изменение размера внутренних массивов _storage
	}
}

template <typename T> Matrix<T> Matrix<T>::SubmatrixOf (uint32_t _row, uint32_t _column) {
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

template <typename T> void Matrix<T>::MultiplyWith(Matrix& _matrix) {
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

template <typename T> T Matrix<T>::DeterminantOf() {
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
			Matrix<T> submatrix = this->SubmatrixOf(0, j);
			if (j % 2 == 0) { // Миноры одной строчки.
				determinant += this->_storage[0][j] * submatrix.DeterminantOf();
			} else { // Для "четных" элементов произведение элемента и детерминанта его минора складывается и наоборот.
				determinant -= this->_storage[0][j] * submatrix.DeterminantOf();
			}
		}
		return determinant;
	}
}

template <typename T> void Matrix<T>::FillStorage(unsigned char mode, T value, T left_border, T right_border) {
	switch (mode) {
		case 'r': {
			std::mt19937 source(value);
			auto distributor = std::uniform_real_distribution<T>(left_border, right_border);
			if (value == 0) {
				std::random_device rd;
				source = std::mt19937(rd());
			}
			for (uint32_t i = 0; i < this->rows; i++) {
				for (uint32_t j = 0; j < this->columns; j++) {
					this->_storage[i][j] = distributor(source);
				}
			}
			break;
		} case 'i': {
			uint32_t i = 0;
			while (i < this->rows && i < this->columns) {
				this->_storage[i][i] = 1;
				i++;
			}
			break;
		} case 'c': {
			for (uint32_t i = 0; i < this->rows; i++) {
				for (uint32_t j = 0; j < this->columns; j++) {
					this->_storage[i][j] = value;
				}
			}
			break;
		}
	}
}

template <typename T> void Matrix<T>::AdditionByMatrix(Matrix<T>& _matrix) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			this->_storage[i][j] += _matrix._storage[i][j];
		}
	}
}

template <typename T> void Matrix<T>::SubtractionByMatrix(Matrix<T>& _matrix) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			this->_storage[i][j] -= _matrix._storage[i][j];
		}
	}
}

template <typename T> void Matrix<T>::MultiplicationByMatrix(Matrix<T>& _matrix) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			this->_storage[i][j] *= _matrix._storage[i][j];
		}
	}
}

template <typename T> void Matrix<T>::DivisionByMatrix(Matrix<T>& _matrix) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			this->_storage[i][j] *= _matrix._storage[i][j];
		}
	}
}

template <typename T> void Matrix<T>::AdditionByScalar(T _value) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			this->_storage[i][j] += _value;
		}
	}
}

template <typename T> void Matrix<T>::SubtractionByScalar(T _value) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			this->_storage[i][j] -= _value;
		}
	}
}

template <typename T> void Matrix<T>::MultiplicationByScalar(T _value) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			this->_storage[i][j] *= _value;
		}
	}
}

template <typename T> void Matrix<T>::DivisionByScalar(T _value) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			this->_storage[i][j] *= _value;
		}
	}
}

/*template <typename T> void Matrix<T>::Transpose() {
	auto* temp_vector = new std::vector<std::vector<T>>;
	temp_vector->resize(this->columns);
	for (auto& row: *temp_vector) {
		row.resize(this->rows);
	}
	for (size_t i = 0; i < this->rows; i++) {
		for (size_t j = 0; j < this->columns; j++) {
			temp_vector[j][i] = this->_storage[i][j];
		}
	}
	this->_storage.swap(*temp_vector);
	size_t temp = this->rows;
	this->rows = this->columns;
	this->columns = temp;
} */