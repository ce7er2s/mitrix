//
// Created by reenie on 22.09.2021.
//

#include <iostream>
#include <string>
#include <vector>
#include <random>

template <typename T> class Matrix {
	uint32_t columns = 0;
	uint32_t rows = 0;
public:
	std::vector<std::vector<T>> _storage;
	std::string name;
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

	void printFormatTo(std::ostream& _ostream = std::cout) {  // АХТУНГ, НЕ ТРОГАТЬ БЛЯТЬ НИКОГДА, Я ВООБЩЕ НЕ ПОНИМАЮ КАК Я ЭТО СДЕЛАЛ
		uint32_t max_length = 0; // Максимальная длина ячейки
		uint32_t element_length; // Длина конкретной ячейки
		std::string spaces; // Строка для отступа в ячейке
		std::string dashes; // Строка для горизонтальных символов


		for (uint32_t i = 0; i < this->rows; i++) {  // цикл поиска максимальной длины ячейки
			for (uint32_t sign, length, j = 0; j < this->columns; j++) {
				T temp = this->_storage[i][j];
				sign = temp>0 ? 0 : 1; // Определение смещения для знаковых и беззнаковых значений
				length = std::to_string(temp).length() + sign; // Длина записи текущей ячейки
				if (length > max_length)
					max_length = length; // Сохранение максимальной ячейки
			}
		}


		std::cout << "╔";
		for (uint32_t j = 0; j < this->columns-1; j++) {
			for (uint32_t d = 0; d <= max_length; d++)
				std::cout << "═"; // Печать разделителя строк и крестиков
			std::cout << "╦";
		}
		for (uint32_t d = 0; d <= max_length; d++) // последие разделители отдельно
			std::cout << "═";
		std::cout << "╗" << std::endl; // Закрывающий разделитель и перевод строки


		for (uint32_t i = 0; i < this->rows-1; i++) {

			for (uint32_t j = 0; j < this->columns; j++) {
				std::string temp = std::to_string(this->_storage[i][j]); // Временная переменная элемента массива
				std::cout << "║"; 				// Печать раздителителя
				element_length = temp.length(); // Длина текущей ячейки;
				spaces = std::string(max_length - element_length, ' ');		// Смещение для печати текущей ячейки
				std::cout << spaces;					// Печать смещения
				std::cout << temp;		// Печать значения
				std::cout << ' ';		// Печать значения
			}
			std::cout << "║" << std::endl; // Закрывающий раздителитель, перевод строки


			std::cout << "╠";  // Разделитель для строк
			for (uint32_t j = 0; j < this->columns-1; j++) {
				for (uint32_t d = 0; d <= max_length; d++)
					std::cout << "═"; // Печать разделителя строк и крестиков
				std::cout << "╬";
			}
			for (uint32_t d = 0; d <= max_length; d++) // последие разделители отдельно
				std::cout << "═";
			std::cout << "╣" << std::endl; // Закрывающий разделитель и перевод строки
		}


		for (uint32_t j = 0; j < this->columns; j++) {
			std::string temp = std::to_string(this->_storage[(this->rows)-1][j]);  // Временная переменная элемента массива
			std::cout << "║"; 				// Печать раздителителя
			element_length = temp.length(); // Длина текущей ячейки;
			spaces = std::string(max_length - element_length, ' ');		// Смещение для печати текущей ячейки
			std::cout << spaces;					// Печать смещения
			std::cout << temp;		// Печать значения
			std::cout << ' '; // Печать отступа
		}
		std::cout << "╣" << std::endl;


		std::cout << "╚"; // Начало отрисовки дна
		for (uint32_t j = 0; j < this->columns-1; j++) {
			for (uint32_t d = 0; d <= max_length; d++)
				std::cout << "═"; // Печать разделителя строк и крестиков
			std::cout << "╩";
		}
		for (uint32_t d = 0; d <= max_length; d++) // последие разделители отдельно
			std::cout << "═";
		std::cout << "╝" << std::endl; // Закрывающий разделитель и перевод строки
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
				  //static std::normal_distribution distributor;
				  std::mt19937 source(value);
				  auto distributor = std::uniform_int_distribution<T>(left_border, right_border);
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
};
