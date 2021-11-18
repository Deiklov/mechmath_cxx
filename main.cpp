#include <iostream>

#include "Matrix/Matrix.hpp"

using namespace std;

void testInputOutput() {
  while (true) {
    int m, n;
    cout << "Enter dimensions of matrix(rows,cols): " << endl;
    cin >> m >> n;
    if (!cin.good()) {
      break;
    }
    Matrix a(m, n);
    cout << "Enter elements of matrix: " << endl;
    cin >> a;
    if (!cin.good()) {
      cout << "Incorrect input" << endl;
      break;
    }
    cout << "Matrix:" << endl;
    cout << a;
    cout << "------------" << endl;
    vector<double> vect{1, 2, 3};
    //    auto solution = a.solveSLAU(Matrix(vect, 3, 1));
    auto inverse = a.inverse();
    cout << "Inverse matrix" << inverse << endl;
    cout << a << endl;
    cout << a.determinant() << endl;
  }
}

int main() {
  testInputOutput();
  return 0;
}
