#include "Matrix.hpp"

#include <sstream>

Matrix Matrix::inverse() const {
  Matrix Id = createIdentity(rows());
  Matrix AI = augment(Id);
  Matrix U = AI.gaussianEliminate();
  //  cout << "U" << endl << U << endl;
  Matrix IAInverse = U.rowReduceFromGaussian();
  //  cout << "IAInverse" << endl << IAInverse << endl;
  Matrix AInverse(rows(), columns());
  for (int i = 0; i < AInverse.rows(); ++i) {
    for (int j = 0; j < AInverse.columns(); ++j) {
      AInverse[i][j] = IAInverse[i][j + columns()];
    }
  }
  return AInverse;
}

double Matrix::determinant() const {
  if (rows() != columns())
    throw range_error("Determinant of non-square matrix is no exist");
  Matrix a = *this;
  auto a2 = a.gaussianEliminate();
  double res = 1.;
  for (int i = 0; i < a.m; ++i) {
    res *= a2[i][i];
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

Matrix Matrix::augment(Matrix B) const {
  Matrix A = *this;
  Matrix AB(A.rows(), A.columns() + B.columns());
  for (int i = 0; i < AB.rows(); ++i) {
    for (int j = 0; j < AB.columns(); ++j) {
      if (j < A.columns())
        AB[i][j] = A[i][j];
      else
        AB[i][j] = B[i][j - B.columns()];
    }
  }
  return AB;
}

Matrix Matrix::solveSLE(Matrix b) const {
  // Gaussian elimination
  auto A = Matrix(this->elems, this->rows(), this->columns());
  for (int i = 0; i < A.rows(); ++i) {
    if (A[i][i] == 0) {
      // pivot 0 - throw error
      throw domain_error(
          "Error: the coefficient matrix has 0 as A pivot. Please fix "
          "the input and try again.");
    }
    for (int j = i + 1; j < A.rows(); ++j) {
      for (int k = i + 1; k < A.columns(); ++k) {
        A[j][k] -= A[i][k] * (A[j][i] / A[i][i]);
        if (A[j][k] < MATRIX_EPS && A[j][k] > -1 * MATRIX_EPS) A[j][k] = 0;
      }
      b[j][0] -= b[i][0] * (A[j][i] / A[i][i]);
      if (A[j][0] < MATRIX_EPS && A[j][0] > -1 * MATRIX_EPS) A[j][0] = 0;
      A[j][i] = 0;
    }
  }
  // Back substitution
  Matrix x(b.rows(), 1);
  x[x.rows() - 1][0] = b[x.rows() - 1][0] / A[x.rows() - 1][x.rows() - 1];
  if (x[x.rows() - 1][0] < MATRIX_EPS && x[x.rows() - 1][0] > -1 * MATRIX_EPS)
    x[x.rows() - 1][0] = 0;
  for (int i = x.rows() - 2; i >= 0; --i) {
    double sum = 0;
    for (int j = i + 1; j < x.rows(); ++j) {
      sum += A[i][j] * x[j][0];
    }
    x[i][0] = (b[i][0] - sum) / A[i][i];
    if (x[i][0] < MATRIX_EPS && x[i][0] > -1 * MATRIX_EPS) x[i][0] = 0;
  }

  return x;
}

Matrix Matrix::gaussianEliminate() const {
  Matrix Ab(*this);
  int rows = Ab.rows();
  int cols = Ab.columns();
  int Acols = cols - 1;

  int i = 0;  // row tracker
  int j = 0;  // column tracker

  // iterate through the rows
  while (i < rows) {
    // find a pivot for the row
    bool pivot_found = false;
    while (j < Acols && !pivot_found) {
      if (Ab[i][j] != 0) {  // pivot not equal to 0
        pivot_found = true;
      } else {  // check for a possible swap
        int max_row = i;
        double max_val = 0;
        for (int k = i + 1; k < rows; ++k) {
          double cur_abs = Ab[k][j] >= 0 ? Ab[k][j] : -1 * Ab[k][j];
          if (cur_abs > max_val) {
            max_row = k;
            max_val = cur_abs;
          }
        }
        if (max_row != i) {
          Ab.swapRows(max_row, i);
          pivot_found = true;
        } else {
          j++;
        }
      }
    }

    // perform elimination as normal if pivot was found
    if (pivot_found) {
      for (int t = i + 1; t < rows; ++t) {
        for (int s = j + 1; s < cols; ++s) {
          Ab[t][s] = Ab[t][s] - Ab[i][s] * (Ab[t][j] / Ab[i][j]);
          if (Ab[t][s] < MATRIX_EPS && Ab[t][s] > -1 * MATRIX_EPS) Ab[t][s] = 0;
        }
        Ab[t][j] = 0;
      }
    }

    i++;
    j++;
  }

  return Ab;
}

Matrix Matrix::rowReduceFromGaussian() const {
  Matrix R(this->elems, this->rows(), this->columns());
  int rows = R.rows();
  int cols = R.columns();

  int i = rows - 1;  // row tracker
  int j = cols - 2;  // column tracker

  // iterate through every row
  while (i >= 0) {
    // find the pivot column
    int k = j - 1;
    while (k >= 0) {
      if (R[i][k] != 0) j = k;
      k--;
    }

    // zero out elements above pivots if pivot not 0
    if (R[i][j] != 0) {
      for (int t = i - 1; t >= 0; --t) {
        for (int s = 0; s < cols; ++s) {
          if (s != j) {
            R[t][s] = R[t][s] - R[i][s] * (R[t][j] / R[i][j]);
            if (R[t][s] < MATRIX_EPS && R[t][s] > -1 * MATRIX_EPS) R[t][s] = 0;
          }
        }
        R[t][j] = 0;
      }

      // divide row by pivot
      for (int k = j + 1; k < cols; ++k) {
        R[i][k] = R[i][k] / R[i][j];
        if (R[i][k] < MATRIX_EPS && R[i][k] > -1 * MATRIX_EPS) R[i][k] = 0;
      }
      R[i][j] = 1;
    }

    i--;
    j--;
  }

  return R;
}

Matrix Matrix::createIdentity(int size) {
  Matrix temp(size, size);
  for (int i = 0; i < temp.rows(); ++i) {
    for (int j = 0; j < temp.columns(); ++j) {
      if (i == j) {
        temp[i][j] = 1;
      } else {
        temp[i][j] = 0;
      }
    }
  }
  return temp;
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

int Matrix::rank() const {
  Matrix A = this->gaussianEliminate();
  int rank = 0;

  for (int i = 0; i < A.rows(); ++i) {
    for (int j = 0; j < A.columns(); ++j) {
      if (A[i][j] != 0) {
        rank++;
        break;
      }
    }
  }

  return rank;
}
