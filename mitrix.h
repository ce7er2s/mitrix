//
// Created by reenie on 22.09.2021.
//

#ifndef UNTITLED_MITRIX_H
#define UNTITLED_MITRIX_H


#include <iostream>
#include <fstream>
#include <string>
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
};


#endif //UNTITLED_MITRIX_H
