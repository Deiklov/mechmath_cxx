#include <iostream>
#include <cmath>
#include "Matrix/Matrix.hpp"
#include <vector>

using namespace std;

int main()
{
    const int rows = 3;
    const int cols = 4;
    vector<vector<double>> matr;
    //    double arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    matr.resize(rows);
    for (int i = 0; i < matr.size(); ++i)
    {
        matr.at(i).resize(cols);
    }
    matr.at(0) = {0, 1, 2, 3};
    matr.at(1) = {4, 5, 6, 7};
    matr.at(2) = {8, 9, 10, 11};

    Matrix matrix(matr, 3, 4);
    matrix.transpose();
    cout << matrix.transpose() << endl;
    return 0;
}