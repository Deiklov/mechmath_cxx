/*
 * matrix.h
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include <vector>

using namespace std;
const double MATRIX_EPS = 1e-12;

class Matrix {
  int m;  // rows
  int n;  // columns
 public:
  int rows() const { return m; }
  int columns() const { return n; }
  vector<double> elems;
  explicit Matrix(int num_rows = 1, int num_columns = 1)
      : m(num_rows), n(num_columns), elems(m * n) {}
  explicit Matrix(const vector<double> &elements, int num_rows, int num_columns)
      : m(num_rows), n(num_columns), elems(m * n) {
    for (int i = 0; i < m * n; ++i) {
      elems[i] = elements.at(i);
    }
  }
  Matrix augment(Matrix A, Matrix B);
  Matrix createIdentity(size_t size);
  Matrix solveSLAU(Matrix b);
  Matrix gaussianEliminate();
  Matrix rowReduceFromGaussian();
  double &at(int i, int j) {
    if (i < 0 || i >= m || j < 0 || j >= n) {
      throw out_of_range("Matrix index out of bounds");
    }
    return elems[i * n + j];
  }
  const double *operator[](int i) const { return elems.data() + i * n; }

  double *operator[](int i) { return elems.data() + i * n; }
  Matrix &reszie(int numRows, int numCols) {
    elems.resize(numRows * numCols);
    m = numRows;
    n = numCols;
    for (int i = 0; i < m * n; ++i) {
      elems[i] = 0.;
    }
    return *this;
  }
  Matrix operator+(const Matrix &b) const;
  Matrix operator-(const Matrix &b) const;
  Matrix operator*(const Matrix &b) const;
  Matrix operator/(const Matrix &b) const;
  Matrix inverse();
  void swapRows(int i, int k);
  void addRows(int i, int k, double coeff);  // line_i+=line_k*coeff;

  int gauss();
  double determinant() const;

  Matrix &operator+=(const Matrix &b) {
    if (m != b.m || n != b.n) {
      throw range_error("Sum of matrices of different dimension");
    }
    Matrix c = *this + b;
    *this = c;
    return *this;
  }
  Matrix &operator-=(const Matrix &b) {
    if (m != b.m || n != b.n) {
      throw range_error("Subtraction of matrices of different dimension");
    }
    Matrix c = *this - b;
    *this = c;
    return *this;
  }
};
ostream &operator<<(ostream &s, const Matrix &a);
istream &operator>>(istream &s, Matrix &a);

#endif
