#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <time.h>

#include "kernels.cuh"

template <class Number>
class Matrix {
public:
  /* Constructors */
  Matrix<Number>(size_t height, size_t width);
  Matrix<Number>(std::initializer_list<std::initializer_list<Number>> array);
  ~Matrix<Number>();

  /* Getters */
  size_t getWidth() const;
  size_t getHeight() const;

  /* Operations */
  Matrix<Number> add(const Matrix<Number>& m) const;
  Matrix<Number> dot(const Matrix<Number>& m) const;
  Matrix<Number> transpose() const;

  /* Operators */
  Matrix<Number> operator + (const Matrix&);
  Matrix<Number> operator * (const Matrix&);

  /* Display */
  void display() const;

private:
  size_t height;
  size_t width;
  Number* array;
};

template <class Number>
Matrix<Number>::Matrix(size_t height, size_t width) {
	static_assert(std::is_same<Number, int>::value ||
		            std::is_same<Number, float>::value ||
            		std::is_same<Number, double>::value,
            		"Type not allowed. Use <int>, <float> or <double>.");
	this->height = height;
	this->width = width;
	this->array = (Number*)calloc(height * width, sizeof(Number));
	srand(time(NULL));
	for (size_t i = 0; i < height * width; i++) {
		this->array[i] = -1 + Number(rand()) / Number(RAND_MAX) * 2;
	}
}

template <class Number>
Matrix<Number>::Matrix(std::initializer_list<std::initializer_list<Number>> arr) {
	static_assert(std::is_same<Number, int>::value ||
            		std::is_same<Number, float>::value ||
            		std::is_same<Number, double>::value,
            		"Type not allowed. Use <int>, <float> or <double>.");
	this->height = (int)(arr.begin())->size();
	this->width = (int)arr.size();
	this->array = (Number*)calloc(this->height * this->width, sizeof(Number));
	srand(time(NULL));
	for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      this->array[i * width + j] = (arr.begin() + i)->begin()[j];
    }
	}
}

template <class Number>
Matrix<Number>::~Matrix() {
	free(this->array);
}

template <class Number>
size_t Matrix<Number>::getHeight() const {
	return this->height;
}

template <class Number>
size_t Matrix<Number>::getWidth() const {
	return this->width;
}

template <class Number>
Matrix<Number> Matrix<Number>::add(const Matrix& m) const {
	Matrix result(height, width);
  Wrapper().add(this->array, m.array, result.array, this->width * this->height);
	return result;
}

template <class Number>
Matrix<Number> Matrix<Number>::dot(const Matrix& m) const {
	Number val = 0;
	Matrix<Number> result(this->height, m.width);
	for (size_t i = 0; i < this->height; i++) {
		for (size_t j = 0; j < m.width; j++) {
			for (size_t h = 0; h < this->width; h++) {
				val += this->array[i * this->width + h] * m.array[h * m.width + j];
			}
			result.array[i * m.width + j] = val;
			val = 0;
		}
	}
	return result;
}

template <class Number>
Matrix<Number> Matrix<Number>::transpose() const {
	Matrix<Number> result(this->width, this->height);
	for (size_t i = 0; i < this->width; i++) {
		for (size_t j = 0; j < this->height; j++) {
			result.array[i * this->height + j] = this->array[j * this->height + i];
		}
	}
	return result;
}

template <class Number>
void Matrix<Number>::display() const {
	for (size_t i = 0; i < this->height; i++) {
		for (size_t j = 0; j < this->width; j++) {
			std::cout << this->array[i * this->width + j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

template <class Number>
Matrix<Number> Matrix<Number>::operator + (const Matrix& m) {
	return this->add(m);
}

template <class Number>
Matrix<Number> Matrix<Number>::operator * (const Matrix& m) {
	return this->dot(m);
}

#endif
