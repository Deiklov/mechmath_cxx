#include "Matrix.hpp"

#include <sstream>

Matrix Matrix::operator+(const Matrix &b) const {
  if (rows() != b.rows() || columns() != b.columns()) {
    throw range_error("Sum of matrices of different dimension");
  }
  Matrix res(m, n);
  for (int i = 0; i < m * n; ++i) {
    res.elems[i] = elems[i] + b.elems[i];
  }
  return res;
}

Matrix Matrix::operator-(const Matrix &b) const {
  if (rows() != b.rows() || columns() != b.columns()) {
    throw range_error("Substrate of matrices of different dimension");
  }
  Matrix res(m, n);
  for (int i = 0; i < m * n; ++i) {
    res.elems[i] = elems[i] - b.elems[i];
  }
  return res;
}

Matrix Matrix::operator*(const Matrix &b) const {
  if (columns() != b.rows())
    throw range_error("Product of matrices of incorrect dimension");
  Matrix res(m, b.n);
  for (int i = 0; i < res.m; ++i) {
    for (int j = 0; j < res.n; ++j) {
      res[i][j] = 0.;
      for (int k = 0; k < n; ++k) {
        res[i][j] += (*this)[i][k] * b[k][j];
      }
    }
  }
  return res;
}

Matrix Matrix::operator/(const Matrix &b) const { return Matrix(); }

Matrix Matrix::inverse() const { return Matrix(); }

Matrix Matrix::gauss() const { return Matrix(); }

double Matrix::determinant() const { return 0; }

ostream &operator<<(ostream &s, const Matrix &a) {
  int maxWidth = 0;
  for (int i = 0; i < a.rows(); ++i) {
    for (int j = 0; j < a.columns(); ++j) {
      ostringstream str;
//      str.str().clear();
      str << a[i][j];
      int w = int(str.str().size());
      if (w > maxWidth) maxWidth = w;
      str.str().clear();
    }
  }

  for (int i = 0; i < a.rows(); ++i) {
    for (int j = 0; j < a.columns(); ++j) {
      if (j > 0) s << " ";
      ostringstream str;
//      str.str().clear();
      str << a[i][j];
      int w = int(str.str().size());
      if (w < maxWidth) {
        for (int k = 0; k < maxWidth - w; ++k) {
          s << " ";
        }
      }
      s << str.str();
    }
    s << endl;
  }
  return s;
}
istream &operator>>(istream &s, Matrix &a) {
  for (int i = 0; s.good() && i < a.rows(); ++i) {
    for (int j = 0; s.good() && j < a.columns(); ++j) {
      s >> a[i][j];
    }
  }
  return s;
}
