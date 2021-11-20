#include <iostream>

#include "Matrix/Matrix.hpp"

using namespace std;

void fullHandler(Matrix& A, vector<double>& bvect) {
  Matrix b(bvect, bvect.size(), 1);

  cout << "Matrix:" << endl;
  cout << A;
  cout << "------------" << endl;

  if (A.rows() == A.columns()) {
    cout << "Determinant of A" << endl;
    cout << A.determinant() << endl;
    cout << "------------" << endl;
    cout << "Inverse of A" << endl;
    cout << A.inverse() << endl;
  }

  cout << "Add A+A:" << endl;
  cout << A + A << endl;
  cout << "------------" << endl;
  cout << "Sub A-Id:" << endl;
  cout << A - Matrix::createIdentity(A.rows()) << endl;
  cout << "------------" << endl;
  cout << "Mul A*A:" << endl;
  cout << A * A << endl;
  cout << "------------" << endl;
  cout << "Row echelon form of A" << endl;
  cout << A.gaussianEliminate() << endl;
  cout << "------------" << endl;
  cout << "Rank of A" << endl;
  cout << A.rank() << endl;

  auto lmda = [&bvect](std::ostream& os) -> std::ostream& {
    for (auto v : bvect) {
      cout << v << " ";
    }
    return os;
  };
  cout << "Solution for ";
  lmda(cout) << " vector" << endl;
  cout << A.solveSLE(b) << endl;
}

void testPredefinedValues() {
  vector<double> bvect{1, 2, 3, 1};
  Matrix A(vector<double>{3, 3.3, -4, -3, 0, 6.5, 1, 1, 5, 4, 2, 1, 2, 3, 3, 2},
           4, 4);
  fullHandler(A, bvect);
}

void testInputOutput() {
  while (true) {
    int m, n;
    cout << "Enter dimensions of matrix(rows,cols): " << endl;
    cin >> m >> n;
    if (!cin.good()) {
      break;
    }
    Matrix A(m, n);
    cout << "Enter elements of matrix: " << endl;
    cin >> A;
    if (!cin.good()) {
      cout << "Incorrect input" << endl;
      break;
    }
    vector<double> bvect(m);
    cout << "Enter constant terms for system of linear equations (must to be "
         << m << " members): " << endl;
    for (int i = 0; i < m; ++i) {
      cin >> bvect[i];
    }
    fullHandler(A, bvect);
  }
}

int main() {
  testInputOutput();
  //  testPredefinedValues();
  return 0;
}
