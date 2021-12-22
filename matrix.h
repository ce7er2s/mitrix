#ifndef MITRIX_H
#define MITRIX_H


template <typename T> class Matrix {
public:
	uint32_t columns = 0;
	uint32_t rows = 0;
	std::vector<std::vector<T>> storage;
	std::wstring name;
public:
	std::vector<T>& operator[](int32_t n) {
		return storage[n];
	}

	Matrix<T>& operator=(const Matrix<T>& matrix) { // оператор копирования
		rows = matrix.rows;
		columns = matrix.columns;
		storage = matrix.storage;
		// name = matrix.name; // имя матрицы не копируется
		return *this;
	}

	Matrix<T>& operator=(Matrix<T>&& matrix) noexcept { // оператор перемещения
		if (this != &matrix) {
			rows = matrix.rows;
			columns = matrix.columns;
			storage.swap(matrix.storage);
		}
		return *this;
	}

	~Matrix() {
		std::vector<std::vector<T>>().swap(storage);
	}


	Matrix(const Matrix<T>& _matrix_1, const Matrix<T>& _matrix_2) {
		*this = _matrix_1;
		this->MultiplyWith(_matrix_2);
	} // инициализация через умножение


	explicit Matrix(uint32_t _rows = 0, uint32_t _columns = 0) {
		rows = _rows;									// стандартная инициализация
		columns = _columns;
		storage.resize(_rows);
		for (size_t i = 0; i < _rows; i++) {
			storage[i].resize(_columns);
		}
	};

	explicit Matrix(std::vector<std::vector<T>>& _storage) { // стандартная инициализация
		rows = _storage.size();
		columns = _storage[0].size();
		storage = std::move(_storage);
	};

	Matrix (const Matrix<T>& matrix) { // конструктор копирования
		rows = matrix.rows;
		columns = matrix.columns;
		storage = matrix.storage;
	}

	Matrix (Matrix<T>&& matrix) noexcept { // конструктор перемещения
		if (this != &matrix) {
			rows = matrix.rows;
			columns = matrix.columns;
			storage.swap(matrix._storage);
			// name = matrix.name;
		}
		return *this;
	}

	Matrix<T>& SubmatrixOf (uint32_t _row, uint32_t _column) {
		auto* temp = new std::vector<std::vector<T>>; // создание минора по индексам
		for (size_t i = 0; i < rows; i++) {
			if (i != _row) {
				(*temp).emplace_back();
			}
			for (size_t j = 0; j < columns; j++) {
				if (i != _row && j != _column) {
					(*temp)[temp->size()-1].emplace_back(storage[i][j]);
				}
			}
		}
		return Matrix<T>(*temp);
	};

	void ResizeTo(uint32_t _rows, uint32_t _columns) {
		rows = _rows;
		columns = _columns;
		for (size_t i = 0; i < rows; i++) {
			storage[i].resize(columns);
			storage[i].shrink_to_fit();
		}
		storage.resize(rows);
		storage.shrink_to_fit();
	};

	void FillStorage(unsigned char mode = 'r', T value = 0, T left_border = 0, T right_border = 0) {
		switch (mode) { // Возможно стоит не писать эту функцию сразу здесь.
			case 'r': {
				std::mt19937 source(value);
				auto distributor = std::uniform_real_distribution<T>(left_border, right_border);
				if (value == 0) {
					std::random_device rd;
					source = std::mt19937(rd());
				}
				for (uint32_t i = 0; i < rows; i++) {
					for (uint32_t j = 0; j < columns; j++) {
						this->storage[i][j] = distributor(source);
					}
				}
				break;
			} case 'i': {
				uint32_t i = 0;
				while (i < rows && i < columns) {
					this->storage[i][i] = 1;
					i++;
				}
				break;
			} case 'c': {
				for (uint32_t i = 0; i < rows; i++) {
					for (uint32_t j = 0; j < columns; j++) {
						this->storage[i][j] = value;
					}
				}
				break;
			} default: {
				;
			}
		}
	}

	void MultiplyWith(Matrix<T>& matrix) { // умножение матрицы
		std::vector<std::vector<T>> temp;
		uint32_t size = matrix.rows;
		uint32_t temp_rows = this->rows;
		uint32_t temp_columns = matrix.columns;
		temp.resize(temp_rows);
		for (uint32_t i = 0; i < size; i++)
			temp[i].resize(temp_columns);
		for (uint32_t i = 0; i < temp_rows; i++) { // Ниибаца тут тройной цикл.
			for (uint32_t j = 0; j < temp_columns; j++) {
				for (uint32_t k = 0; k < size; k++)
					temp[i][j] += storage[i][k] * matrix.storage[k][j];
			}
		}
		storage.swap(temp);
		columns = temp_columns;
	};

	void MultiplicationByMatrix(Matrix<T>& matrix) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				storage[i][j] *= matrix.storage[i][j];
			}
		}

	}

	void AdditionByMatrix(Matrix<T>& matrix) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				storage[i][j] += matrix.storage[i][j];
			}
		}
	};

	void SubtractionByMatrix(Matrix<T>& matrix) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				storage[i][j] -= matrix.storage[i][j];
			}
		}
	};

	void DivisionByMatrix(Matrix<T>& matrix) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				if (matrix.storage[i][j] == 0) {
					if (std::signbit(storage[i][j])) {
						storage[i][j] = -std::numeric_limits<double>::quiet_NaN();
					} else {
						storage[i][j] = std::numeric_limits<double>::quiet_NaN();
					}
				} else {
					storage[i][j] /= matrix.storage[i][j];
				}
			}
		}
	};

	void MultiplicationByScalar(T value) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				storage[i][j] *= value;
			}
		}
	};
	void AdditionByScalar(T value) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				storage[i][j] += value;
			}
		}
	};
	void SubtractionByScalar(T value) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				storage[i][j] -= value;
			}
		}
	};
	void DivisionByScalar(T value) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				if (value == 0) {
					if (std::signbit(storage[i][j])) {
						storage[i][j] = -std::numeric_limits<double>::quiet_NaN();
					} else {
						storage[i][j] = std::numeric_limits<double>::quiet_NaN();
					}
				} else {
					storage[i][j] /= value;
				}
			}
		}
	};

	std::vector<Matrix<T>*> lu_transform() {};

	void Transpose() {
		std::vector<std::vector<T>>new_matrix_data(this->columns); // временная переменная
		for (std::vector<T>& row: new_matrix_data) {
			row.resize(this->rows); // меняем размеры на обратные
		}
		for (size_t i = 0; i < this->columns; i++) {
			for (size_t j = 0; j < this->rows; j++) {
				new_matrix_data[i][j] = this->_storage[j][i];
			}
		}
		storage.swap(new_matrix_data);
		std::swap(rows, columns);
	};

	T DeterminantOf() {
		if (rows == 2) { // Конечный случай для 2x2 матрицы
			return (storage[0][0]*storage[1][1]) - (storage[0][1]*storage[1][0]);
		} else if (rows == 1) {
			return storage[0][0]; // Отдельный случай.
		} else {
			T determinant = 0;
			for (uint32_t j = 0; j < columns; j++) {
				// Зачем умножать, если это ноль?
				if (storage[0][j] == 0) {
					continue;
				}
				Matrix<T> submatrix = SubmatrixOf(0, j);
				if (j % 2 == 0) { // Миноры одной строчки.
					determinant += storage[0][j] * submatrix.DeterminantOf();
				} else { // Для "четных" элементов произведение элемента и детерминанта его минора складывается и наоборот.
					determinant -= storage[0][j] * submatrix.DeterminantOf();
				}
			}
			return determinant;
		}
	};
};


#endif // MITRIX_H]
