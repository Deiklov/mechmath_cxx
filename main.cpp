#include <cmath>
#include <iostream>
#include <vector>

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
  }
}

int main() {
  testInputOutput();
  return 0;
}
