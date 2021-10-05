//
// Created by reenie on 22.09.2021.
//
#include <iostream>
#include <iomanip>
#include <string>
#include <random>

template <typename T> Matrix<T>::Matrix(Matrix<T> _matrix_1, Matrix _matrix_2) { // умножение на основе метода multiplyWith
	this->rows = _matrix_1.rows;		// Плохо работает.
	this->columns = _matrix_2.columns;
	this->_storage.swap(_matrix_1._storage);
	this->multiplyWith(_matrix_2);
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

template <typename T> void Matrix<T>::FillStorage(char mode, T value, T left_border, T right_border) {
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
		default: {
			std::cerr << "fill mode " << mode << " is not specified" << std::endl;
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




/* template <typename T> T Matrix<T>::operator~() {
	return this->determinantOf();
}

template <typename T> std::vector<T> Matrix<T>::operator[] (int32_t n) {
	return this->_storage[n];	// TODO: Реализация доступа к приватному _storage по отрицательному индексу aka Python-style
}
template <typename T> Matrix<T> Matrix<T>::operator* (Matrix& _matrix) { // Надо проверить как я в принципе передаю параметры.
	return Matrix<T>(*this, _matrix);	//  Плохо работает.
}
template <typename T> void Matrix<T>::operator*= (Matrix& _matrix) {
	this->multiplyWith(_matrix);
} */

/* template <typename T> void Matrix<T>::inputFrom(std::istream& _istream, uint32_t _rows, uint32_t _columns) {
	if (_rows == 0 && _columns == 0) {
		_istream >> _rows;
		_istream >> _columns;
	}
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

template <typename T> void Matrix<T>::printTo(std::ostream& _ostream) {
	for (uint32_t i = 0; i < this->rows; i++) {
		for (uint32_t j = 0; j < this->columns; j++) {
			_ostream << this->_storage[i][j];
			_ostream << " "; // TODO: сделать разделитель отдельным атрибутом
		}
		_ostream << std::endl;
	}
}

template <typename T> void Matrix<T>::printFormatTo(std::ostream& _ostream, char precision) {  // АХТУНГ, НЕ ТРОГАТЬ БЛЯТЬ НИКОГДА, Я ВООБЩЕ НЕ ПОНИМАЮ КАК Я ЭТО СДЕЛАЛ
	uint32_t max_length = 0; // Максимальная длина ячейки
	std::string spaces; // Строка для отступа в ячейке
	std::string dashes; // Строка для горизонтальных символов

	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision);
	for (uint32_t i = 0; i < this->rows; i++) {  // цикл поиска максимальной длины ячейки
		for (uint32_t length, j = 0; j < this->columns; j++) {
			stream << this->_storage[i][j];
			std::string temp = stream.str();
			stream.str("");
			length = temp.length(); // Длина записи текущей ячейки
			if (length > max_length)
				max_length = length; // Сохранение максимальной ячейки
		}
	}

	_ostream << "╔";
	for (uint32_t j = 1; j < this->columns; j++) {
		for (uint32_t d = 0; d < max_length + 2; d++)
			_ostream << "═"; // Печать разделителя строк и крестиков
		_ostream << "╦";
	}
	for (uint32_t d = 0; d < max_length + 2; d++) // последие разделители отдельно (2 потому что отступ в один пробел с двух сторон)
		_ostream << "═";
	_ostream << "╗" << std::endl; // Закрывающий разделитель и перевод строки


	for (uint32_t i = 1; i < this->rows; i++) {
		_ostream << "║ ";
		for (uint32_t j = 0; j < this->columns; j++) {
			stream << this->_storage[i-1][j];
			std::string temp = stream.str();// Временая переменная элемента массива
			stream.str("");
			spaces = std::string(max_length - temp.length(), ' ');		// Смещение для печати текущей ячейки
			_ostream << spaces;											// Печать смещения
			_ostream << temp;												// Печать значения
			if (j != (this->columns)-1)
				_ostream << " ║ "; 											// Печать раздителителя
		}
		_ostream << " ║" << std::endl; // Перевод строки


		_ostream << "╠";  // Разделитель для строк
		for (uint32_t j = 1; j < this->columns; j++) {
			for (uint32_t d = 0; d < max_length + 2; d++) // (2 потому что отступ в один пробел с двух сторон)
				_ostream << "═"; // Печать разделителя строк и крестиков
			_ostream << "╬";
		}
		for (uint32_t d = 0; d < max_length + 2; d++) // последие разделители отдельно (2 потому что отступ в один пробел с двух сторон)
			_ostream << "═";
		_ostream << "╣" << std::endl; // Закрывающий разделитель и перевод строки
	}

	_ostream << "║ ";  // Печать последней строки
	for (uint32_t j = 0; j < this->columns; j++) {
		stream << this->_storage[(this->rows)-1][j];
		std::string temp = stream.str();									    // Временая переменная элемента массива
		stream.str("");
		spaces = std::string(max_length - temp.length(), ' ');				// Смещение для печати текущей ячейки
		_ostream << spaces;													// Печать смещения
		_ostream << temp;														// Печать значения
		if (j != (this->columns)-1)
			_ostream << " ║ "; 													// Печать раздителителя
	}
	_ostream << " ║" << std::endl; // печать последнего разделителя

	_ostream << "╚"; // Начало отрисовки дна
	for (uint32_t j = 1; j < this->columns; j++) {
		for (uint32_t d = 0; d < max_length + 2; d++) // (2 потому что отступ в один пробел с двух сторон)
			_ostream << "═"; // Печать разделителя строк и крестиков
		_ostream << "╩";
	}
	for (uint32_t d = 0; d < max_length + 2; d++) // последие разделители отдельно (2 потому что отступ в один пробел с двух сторон)
		_ostream << "═";
	_ostream << "╝" << std::endl; // Закрывающий разделитель и перевод строки
} */