#include <iostream>
#include <cmath>
#include <cassert>
#include "R2Graph/R2Graph.h"

using namespace std;

auto &calculateOrthocenter(const R2Point &a, const R2Point &b, const R2Point &c)
{
    R2Vector normalab = (b - a).normal();
    R2Vector normalac = (c - a).normal();
    R2Point resultPoint;

    intersectStraightLines(c, normalab, b, normalac, resultPoint);
    return resultPoint;
}

int main()
{
    R2Point a, b, c;
    cout << "Enter vertices: " << endl;
    cin >> a >> b >> c;
    assert(cin.good());

    auto result = calculateOrthocenter(a, b, c);
    cout << "Orthocenter point: " << result << endl;

    return 0;
}
