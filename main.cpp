#include <iostream>
#include <cmath>
#include "R2Graph/R2Graph.h"
#include <cassert>

using namespace std;

auto &CalculateOrthocenter(const R2Point &a, const R2Point &b, const R2Point &c)
{
    R2Vector ab = (b - a).normal();
    R2Vector ac = (c - a).normal();
    R2Point resultPoint;

    intersectStraightLines(c, ab, b, ac, resultPoint);
    return resultPoint;
}

int main()
{
    R2Point a, b, c;
    cout << "Enter vertices: " << endl;
    cin >> a >> b >> c;
    assert(cin.good());

    auto result = CalculateOrthocenter(a, b, c);
    cout << "Orthocenter point: " << result << endl;

    return 0;
}
