#pragma once
#include <stdlib.h>
#include <iostream>
#include <assert.h>

template<class T>
class Matrix
{
public:
	int rows, cols;

	T* data = nullptr;

public:
	Matrix(int _rows, int _cols)
		: rows(_rows), cols(_cols) 
	{
		assert(_rows != 0 && _cols != 0);
		data = new T[_rows * _cols];
	}

	Matrix(const Matrix& matrix)
		: rows(matrix.rows), cols(matrix.cols) 
	{
		data = new T[rows * cols];
		*this = matrix;
	}

	void operator=(const Matrix& matrix) {
		rows = matrix.rows;
		cols = matrix.cols;

		for (int i = 0; i < rows * cols; i++) {
			this->data[i] = matrix.data[i];
		}
	}

	~Matrix() {
		delete[] data;
	}


	T get(int y, int x) const {
		return data[cols * (y - 1) + (x - 1)];
	}

	T get(int x) const {
		return data[x];
	}

	void set(int y, int x, T value) {
		data[cols * (y - 1) + (x - 1)] = value;
	}

	void set(int x, T value) {
		data[x] = value;
	}

	static Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
		assert(matrix1.cols == matrix2.rows);

		Matrix result(matrix1.rows, matrix2.cols);

		for (int m1_row = 1; m1_row <= matrix1.rows; m1_row++) {
			for (int m2_col = 1; m2_col <= matrix2.cols; m2_col++) {
				T val = 0;
				for (int i = 1; i <= matrix1.cols; i++) {
					val += matrix1.get(m1_row, i) * matrix2.get(i, m2_col);
				}
				result.set(m1_row, m2_col, val);
			}
		}
		return result;
	}
	Matrix operator*(const Matrix& M) {
		return Matrix::multiply(*this, M);
	}
	void operator*=(const Matrix& M) {
		*this = *this * M;
	}

	static Matrix hadamardProduct(const Matrix& matrix1, const Matrix& matrix2) {
		assert(matrix1.rows == matrix2.rows && matrix1.cols == matrix2.cols);

		Matrix result(matrix1.rows, matrix1.cols);

		for (int i = 1; i <= matrix1.rows; i++) {
			for (int j = 1; j <= matrix1.cols; j++) {
				result.set(i, j, matrix1.get(i, j) * matrix2.get(i, j));
			}
		}
		return result;
	}

	static Matrix add(const Matrix& matrix1, const Matrix& matrix2) {
		assert(matrix1.rows == matrix2.rows && matrix1.cols == matrix2.cols);

		Matrix result(matrix1.rows, matrix1.cols);

		for (int i = 1; i <= matrix1.rows; i++) {
			for (int j = 1; j <= matrix1.cols; j++) {
				result.set(i, j, matrix1.get(i, j) + matrix2.get(i, j));
			}
		}
		return result;
	}
	Matrix operator+(const Matrix& M) {
		return Matrix::add(*this, M);
	}
	void operator+=(const Matrix& M) {
		*this = *this + M;
	}

	static Matrix substract(const Matrix& matrix1, const Matrix& matrix2) {
		assert(matrix1.rows == matrix2.rows && matrix1.cols == matrix2.cols);

		Matrix result(matrix1.rows, matrix1.cols);

		for (int i = 1; i <= matrix1.rows; i++) {
			for (int j = 1; j <= matrix1.cols; j++) {
				result.set(i, j, matrix1.get(i, j) - matrix2.get(i, j));
			}
		}
		return result;
	}
	Matrix operator-(const Matrix& M) {
		return Matrix::substract(*this, M);
	}
	void operator-=(const Matrix& M) {
		*this = *this - M;
	}

	static Matrix transpose(const Matrix& matrix) {
		Matrix result(matrix.cols, matrix.rows);

		for (int i = 1; i <= matrix.rows; i++) {
			for (int j = 1; j <= matrix.cols; j++) {
				result.set(j, i, matrix.get(i, j));
			}
		}
		return result;
	}

	void map(T(*func)(T)) {
		for (int i = 1; i <= rows; i++) {
			for (int j = 1; j <= cols; j++) {
				this->set(i, j, func(this->get(i, j)));
			}
		}
	}

	void fill(T* data) {
		delete[] this->data;
		this->data = data;
	}

	void identity() {
		assert(rows == cols);

		int j = 0;
		for (int i = 0; i < rows * cols; i++) {
			if (j == i) {
				data[i] = 1;
				j += cols + 1;
			} else {
				data[i] = 0;
			}
		}
	}

	void randomize() {
		for (int i = 0; i < rows * cols; i++) {
			data[i] = (T)((((float)rand() / (float)RAND_MAX) * 2) - 1);
		}
	}

	void print() const {
		for (int i = 1; i <= rows * cols; i++) {
			std::cout << data[i - 1] << " ";
			if (i % cols == 0) {
				std::cout << "\n";
			}
		}
	}

};