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

int Matrix::gauss() {
  int i = 0;
  int j = 0;
  while (i < m && j < n) {
    double maxValue = fabs(at(i, j));
    int maxRow = i;
    for (int k = i + 1; k < m; ++k) {
      if (fabs(at(k, j)) > maxValue) {
        maxValue = fabs(at(k, j));
        maxRow = k;
      }
    }
    if (maxValue <= MATRIX_EPS) {
      for (int k = i; k < m; ++k) {
        at(k, j) = 0.;
      }
      ++j;
      continue;
    }
    assert(fabs(at(maxRow, j)) > MATRIX_EPS);
    if (maxRow != i) {
      swapRows(i, maxRow);
    }
    assert(fabs(at(i, j)) > MATRIX_EPS);
    double r = at(i, j);
    for (int k = i + 1; k < m; ++k) {
      addRows(k, i, -at(k, j) / r);
      at(k, j);
    }
    ++i;
    ++j;
  }
  return i;
}

double Matrix::determinant() const {
  if (rows() != columns())
    throw range_error("Determinant of non-square matrix is no exist");
  Matrix a = *this;
  a.gauss();
  double res = 1.;
  for (int i = 0; i < a.m; ++i) {
    res *= a[i][i];
  }
  return res;
}

void Matrix::swapRows(int i, int k) {
  if (i < 0 || i >= m || k < 0 || k >= n)
    throw range_error("Incorrect indices of matrix rows");
  for (int j = 0; j < n; ++j) {
    double tmp = at(i, j);
    at(i, j) = at(k, j);
    at(k, j) = (-tmp);
  }
}

void Matrix::addRows(int i, int k, double coeff) {
  if (i < 0 || i >= m || k < 0 || k >= n)
    throw range_error("Incorrect indices of matrix rows");
  for (int j = 0; j < n; ++j) {
    at(i, j) += at(k, j) * coeff;
  }
}

double round_up(const double value, const int decimal_places = 6) {
  const double multiplier = pow(10.0, decimal_places);
  return ceil(value * multiplier) / multiplier;
}

ostream &operator<<(ostream &s, const Matrix &a) {
  int maxWidth = 0;
  for (int i = 0; i < a.rows(); ++i) {
    for (int j = 0; j < a.columns(); ++j) {
      ostringstream str;
      //      str.str().clear();
      str << round_up(a[i][j]);
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
      str << round_up(a[i][j]);
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
