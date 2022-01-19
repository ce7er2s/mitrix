#ifndef MITRIX_H
#define MITRIX_H

int PERF_COUNTER;

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
			storage.swap(matrix.storage);
			// name = matrix.name;
		}
	}

	Matrix<T> SubmatrixOf (uint32_t _row, uint32_t _column) {
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
		columns = _columns;   // <
		storage.resize(rows); // Эти две строчки были под циклом. Все падало. Я поставил их сюда и теперь всё работает. WTF
		storage.shrink_to_fit();
		for (size_t i = 0; i < rows; i++) {
			storage[i].resize(columns);
			storage[i].shrink_to_fit();
		}
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

	void MultiplyWith(const Matrix<T>& matrix) { // умножение матрицы
		std::vector<std::vector<T>> temp;
		uint32_t size = matrix.rows;
		uint32_t temp_rows = rows;
		uint32_t temp_columns = matrix.columns;
		temp.resize(temp_rows);
		for (uint32_t i = 0; i < size; i++)
			temp[i].resize(temp_columns);
		for (uint32_t i = 0; i < temp_rows; i++) { // Ниибаца тут тройной цикл.
			for (uint32_t j = 0; j < temp_columns; j++) {
				for (uint32_t k = 0; k < size; k++, PERF_COUNTER++) //
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

	void Transpose() {
		std::vector<std::vector<T>>new_matrix_data(columns); // временная переменная
		for (std::vector<T>& row: new_matrix_data) {
			row.resize(rows); // меняем размеры на обратные
		}
		for (size_t i = 0; i < columns; i++) {
			for (size_t j = 0; j < rows; j++) {
				new_matrix_data[i][j] = storage[j][i];
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

template <typename T> Matrix<T> gauss_method(Matrix<T> A_matrix, Matrix<T> B_matrix) {
	auto tempX = Matrix<T>(B_matrix);

	for (size_t j = 0; j < A_matrix.columns-1; j++) { // перебор от нулевого столбца до предпоследнего
		for (size_t i = 1; i < A_matrix.rows; i++) { // перебор с первой строчки до последней
			if (i > j) {
				if (A_matrix.storage[j][j] == 0) {
					throw L"Деление на ноль.";
				}
				T weight = A_matrix.storage[i][j] / A_matrix.storage[j][j]; // коэффициент для i-той строки
				PERF_COUNTER++;
				A_matrix.storage[i][j] = 0;
				for (size_t k = j+1; k < A_matrix.columns; k++, PERF_COUNTER++) {
					A_matrix.storage[i][k] -= weight * A_matrix.storage[j][k]; // вычитание j-той строки, умноженной на коэффициент, из i-той
				}
				B_matrix.storage[i][0] -= weight * B_matrix.storage[j][0];
				PERF_COUNTER++;
			}
		}
	}
	// std::cout << "\n" << A_matrix.DeterminantOf() << " " <<  A_matrix.DeterminantOf() << "\n\n";
	for (size_t i = A_matrix.rows-1; i != 0; i--, PERF_COUNTER++) { // i и Matrix::rows это uint32_t. Они не могут быть меньше нуля
		T x = B_matrix.storage[i][0] / A_matrix.storage[i][i]; // находим нижний X
		for (size_t j = 0; j < i; j++, PERF_COUNTER++) {
			B_matrix.storage[j][0] -= x * A_matrix.storage[j][i]; // вычитаем разницу из свободного члена
			//A_matrix.storage[j][i] = 0; //  обнуляем коэффициент (он уже вынесен как разница со свободным членом)
		}
		tempX.storage[i][0] = x; // записываем X
	}
	if (A_matrix.storage[0][0] == 0) {
		throw L"Деление на ноль.";
	}
	tempX.storage[0][0] = B_matrix.storage[0][0] / A_matrix.storage[0][0]; // поэтому тут случай для нуля
	PERF_COUNTER++;

	return tempX;
}

template <typename T> std::vector<Matrix<T>> lu_transform(Matrix<T>& A_matrix) {
	auto tempL = Matrix<T>(A_matrix.rows, A_matrix.columns);
	auto tempU = Matrix<T>(A_matrix);
	for (size_t i = 0; i < A_matrix.rows; i++) {
		tempL.storage[i][i] = 1;
		//tempU.storage[i][i] = A_matrix.storage[i][i];
	}
	for (size_t j = 0; j < tempU.columns-1; j++) { // перебор от нулевого столбца до предпоследнего
		for (size_t i = 1; i < tempU.rows; i++) { // перебор с первой строчки до последней
			if (i > j) {
				if (tempU.storage[j][j] == 0) {
					throw L"Деление на ноль.";
				}
				T weight = tempU.storage[i][j] / tempU.storage[j][j]; // коэффициент для i-той строки
				PERF_COUNTER++;
				tempU.storage[i][j] = 0;
				for (size_t k = j+1; k < tempU.columns; k++, PERF_COUNTER++) {
					tempU.storage[i][k] -= weight * tempU.storage[j][k]; // вычитание j-той строки, умноженной на коэффициент, из i-той
				}
				tempL.storage[i][j] = weight;
			}
		}
	}
	return {tempL, tempU};
}

template <typename T> Matrix<T> lubx_method(Matrix<T> L_matrix, Matrix<T> U_matrix, Matrix<T> B_matrix) {
	auto tempX = Matrix<T>(B_matrix);
	auto tempY = Matrix<T>(B_matrix);
	T x;
	// прямая подстановка для LY = B
	for (size_t i = 0; i < L_matrix.rows; i++) { // i и Matrix::rows это uint32_t. Они не могут быть меньше нуля
		if (B_matrix.storage[i][0] == 0) {
			throw L"Деление на ноль.";
		}
		for (size_t j = i+1; j < L_matrix.rows; j++, PERF_COUNTER++) {
			B_matrix.storage[j][0] -= B_matrix.storage[i][0] * L_matrix.storage[j][i]; // вычитаем разницу из свободного члена
			//L_matrix.storage[j][i] = 0; //  обнуляем коэффициент (он уже вынесен как разница со свободным членом)
		}
		tempY.storage[i][0] = B_matrix.storage[i][0]; // записываем X
	}
	// обратная подстановка UX = Y

	for (size_t i = U_matrix.rows-1; i != 0; i--) { // i и Matrix::rows это uint32_t. Они не могут быть меньше нуля
		if (U_matrix.storage[i][i] == 0) {
			throw L"Деление на ноль.";
		}
		x = tempY.storage[i][0] / U_matrix.storage[i][i]; // находим нижний X
		PERF_COUNTER++;
		for (size_t j = 0; j < i; j++, PERF_COUNTER++) {
			tempY.storage[j][0] -= x * U_matrix.storage[j][i]; // вычитаем разницу из свободного члена
			U_matrix.storage[j][i] = 0; //  обнуляем коэффициент (он уже вынесен как разница со свободным членом)
		}
		tempX.storage[i][0] = x; // записываем X
	}
	if (U_matrix.storage[0][0] == 0) {
		throw L"Деление на ноль.";
	}
	tempX.storage[0][0] = tempY.storage[0][0] / U_matrix.storage[0][0]; // поэтому тут случай для нуля
	PERF_COUNTER++;

	return tempX;
}

template <typename T> void straight_permutation(Matrix<T>& L_matrix, Matrix<T> B_matrix, Matrix<T>& C_matrix) {
    T x;                                                                                        // Запись в B_matrix как в копию
    // прямая подстановка для LY = B
    for (size_t i = 0; i < L_matrix.rows; i++) { // i и Matrix::rows это uint32_t. Они не могут быть меньше нуля
        if (B_matrix.storage[i][0] == 0) {
            throw L"Деление на ноль.";
        }
        for (size_t j = i+1; j < L_matrix.rows; j++, PERF_COUNTER++) {
            B_matrix.storage[j][0] -= B_matrix.storage[i][0] * L_matrix.storage[j][i]; // вычитаем разницу из свободного члена
            //L_matrix.storage[j][i] = 0; //  обнуляем коэффициент (он уже вынесен как разница со свободным членом)
        }
        C_matrix.storage[i][0] = B_matrix.storage[i][0]; // записываем X
    }
}

template <typename T> void reverse_permutation(Matrix<T>& U_matrix, Matrix<T> C_matrix, Matrix<T>& X_matrix) {// Запись в C_matrix как в копию
    // прямая подстановка для LY = B
    T x;
    for (size_t i = U_matrix.rows-1; i != 0; i--) { // i и Matrix::rows это uint32_t. Они не могут быть меньше нуля
        if (U_matrix.storage[i][i] == 0) {
            throw L"Деление на ноль.";
        }
        x = C_matrix.storage[i][0] / U_matrix.storage[i][i]; // находим нижний X
        PERF_COUNTER++;
        for (size_t j = 0; j < i; j++, PERF_COUNTER++) {
            C_matrix.storage[j][0] -= x * U_matrix.storage[j][i]; // вычитаем разницу из свободного члена
            //U_matrix.storage[j][i] = 0; //  обнуляем коэффициент (он уже вынесен как разница со свободным членом)
        }
        X_matrix.storage[i][0] = x; // записываем X
    }
    if (U_matrix.storage[0][0] == 0) {
        throw L"Деление на ноль.";
    }
    X_matrix.storage[0][0] = C_matrix.storage[0][0] / U_matrix.storage[0][0]; // поэтому тут случай для нуля
    PERF_COUNTER++;
}

#endif // MITRIX_H]
