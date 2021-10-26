#ifndef R3GRAPH_H
#define R3GRAPH_H

#include <cmath>
#include <iostream>

namespace R3Graph {

class R3Vector;
class R3Point;

const double R3_EPSILON = 1.0e-14;

class R3Vector {
public:
    double x;
    double y;
    double z;

    R3Vector():
        x(0.0),
        y(0.0),
        z(0.0)
    {}

    R3Vector(const R3Vector& v):
        x(v.x),
        y(v.y),
        z(v.z)
    {}

    R3Vector(double xx, double yy, double zz):
        x(xx),
        y(yy),
        z(zz)
    {}

    R3Vector& operator=(const R3Vector& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    R3Vector operator+(const R3Vector& v) const {
        return R3Vector(x + v.x, y + v.y, z + v.z);
    }

    R3Vector& operator+=(const R3Vector& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    R3Vector operator-(const R3Vector& v) const {
        return R3Vector(x - v.x, y - v.y, z - v.z);
    }

    R3Vector& operator-=(const R3Vector& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    R3Vector operator-() const {
        return R3Vector(-x, -y, -z);
    }

    R3Vector& negate() {
        x = (-x);
        y = (-y);
        z = (-z);
        return *this;
    }

    R3Vector& invert() {
        return negate();
    }

    double length2() const {
        return (x*x + y*y + z*z);
    }

    double length() const {
        return sqrt(x*x + y*y + z*z);
    }

    R3Vector& normalize() {
        double l = length();
        if (fabs(l) > R3_EPSILON) {
            x /= l;
            y /= l;
            z /= l;
        }
        return *this;
    }

    R3Vector normalized() const { // Return a vector with the same direction
                                  // and unit length
        R3Vector v = *this;
        v.normalize();
        return v;
    }

    double scalarProduct(const R3Vector& v) const {
        return (x*v.x + y*v.y + z*v.z);
    }

    double operator*(const R3Vector& v) const {
        return scalarProduct(v);
    }

    R3Vector operator*(double c) const {
        return R3Vector(x*c, y*c, z*c);
    }

    R3Vector& operator*=(double c) {
        x *= c;
        y *= c;
        z *= c;
        return *this;
    }

    R3Vector vectorProduct(const R3Vector& v) const {
        return R3Vector(
            y*v.z - z*v.y,
            -x*v.z + z*v.x,
            x*v.y - y*v.x
        );
    }

    static double signedVolume(
        const R3Vector& v0, const R3Vector& v1, const R3Vector& v2
    ) {
        return (
            v0.x * v1.y * v2.z +
            v0.y * v1.z * v2.x +
            v0.z * v1.x * v2.y - (
                v0.z * v1.y * v2.x +
                v0.y * v1.x * v2.z +
                v0.x * v1.z * v2.y
            )
        );
    }

    static double volume(
        const R3Vector& v0, const R3Vector& v1, const R3Vector& v2
    ) {
        return fabs(signedVolume(v0, v1, v2));
    }

    double angle(const R3Vector& v) const {
        double len0 = length();
        double len1 = v.length();

        /*...
        if (len0 > R3_EPSILON && len1 > R3_EPSILON)
            return acos(
                scalarProduct(v) / (len0*len1)
            );
        else
            return 0.;
        ...*/

        if (len0 <= R3_EPSILON || len1 <= R3_EPSILON)
            return 0;
        R3Vector ex = (*this) * (1./len0);
        R3Vector w = v * (1./len1);
        R3Vector ez = ex.vectorProduct(w);
        double lenz = ez.length();
        if (lenz <= R3_EPSILON)
            return 0.;
        ez *= (1./lenz);
        R3Vector ey = ez.vectorProduct(ex);
        return fabs(atan2(
            w*ey, w*ex
        ));
    }

    static double angle(
        const R3Vector& v0, const R3Vector& v1
    ) {
        return v0.angle(v1);
    }

    double area(const R3Vector& v) const {
        R3Vector w = vectorProduct(v);
        return w.length();
    }

    static double area(
        const R3Vector& v0, const R3Vector& v1
    ) {
        return v0.area(v1);
    }

    bool operator==(const R3Vector& v) const {
        return (
            fabs(x - v.x) <= R3_EPSILON &&
            fabs(y - v.y) <= R3_EPSILON &&
            fabs(z - v.z) <= R3_EPSILON
        );
    }

    bool operator!=(const R3Vector& v) const {
        return !(*this == v);
    }

    bool operator<(const R3Vector& v) const {
        return (
            x < v.x || (
                x <= v.x && (
                    y < v.y || (
                        y <= v.y && z < v.z
                    )
                )
            )
        );
    }

    bool operator<=(const R3Vector& v) const {
        return (
            x < v.x || (
                x <= v.x && (
                    y < v.y || (
                        y <= v.y && z <= v.z
                    )
                )
            )
        );
    }

    bool operator>(const R3Vector& v) const {
        return !(*this <= v);
    }

    bool operator>=(const R3Vector& v) const {
        return !(*this < v);
    }
};

inline R3Vector operator*(double c, const R3Vector& v) {
    return R3Vector(c*v.x, c*v.y, c*v.z);
}

class R3Point {
public:
    double x;
    double y;
    double z;

    R3Point():
        x(0.0),
        y(0.0),
        z(0.0)
    {}

    R3Point(const R3Point& v):
        x(v.x),
        y(v.y),
        z(v.z)
    {}

    R3Point(double xx, double yy, double zz):
        x(xx),
        y(yy),
        z(zz)
    {}

    R3Point& operator=(const R3Point& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    R3Point operator+(const R3Vector& v) const {
        return R3Point(x + v.x, y + v.y, z + v.z);
    }

    //... R3Point operator+(const R3Point& v) const {
    //...     return R3Point(x + v.x, y + v.y, z + v.z);
    //... }

    R3Point& operator+=(const R3Vector& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    //... R3Point& operator+=(const R3Point& v) {
    //...     x += v.x;
    //...     y += v.y;
    //...     z += v.z;
    //...     return *this;
    //... }

    R3Vector operator-(const R3Point& v) const {
        return R3Vector(x - v.x, y - v.y, z - v.z);
    }

    R3Point operator-(const R3Vector& v) const {
        return R3Point(x - v.x, y - v.y, z - v.z);
    }

    R3Point& operator-=(const R3Vector& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    //... R3Point& operator-=(const R3Point& v) {
    //...     x -= v.x;
    //...     y -= v.y;
    //...     z -= v.z;
    //...     return *this;
    //... }

    //... R3Point operator-() const {
    //...     return R3Point(-x, -y, -z);
    //... }

    //... R3Point& negate() {
    //...     x = (-x);
    //...     y = (-y);
    //...     z = (-z);
    //...     return *this;
    //... }

    //... R3Point operator*(double c) const {
    //...     return R3Point(x*c, y*c, z*c);
    //... }

    //... R3Point& operator*=(double c) {
    //...     x *= c;
    //...     y *= c;
    //...     z *= c;
    //...     return *this;
    //... }

    double distance(const R3Point& p) const {
        return (p - *this).length();
    }

    static double distance(
        const R3Point& p0, const R3Point& p1
    ) {
        return p0.distance(p1);
    }

    bool operator==(const R3Point& v) const {
        return (
            fabs(x - v.x) <= R3_EPSILON &&
            fabs(y - v.y) <= R3_EPSILON &&
            fabs(z - v.z) <= R3_EPSILON
        );
    }

    bool operator!=(const R3Point& v) const {
        return !(*this == v);
    }

    bool operator<(const R3Point& v) const {
        return (
            x < v.x || (
                x <= v.x && (
                    y < v.y || (
                        y <= v.y && z < v.z
                    )
                )
            )
        );
    }

    bool operator<=(const R3Point& v) const {
        return (
            x < v.x || (
                x <= v.x && (
                    y < v.y || (
                        y <= v.y && z <= v.z
                    )
                )
            )
        );
    }

    bool operator>(const R3Point& v) const {
        return !(*this <= v);
    }

    bool operator>=(const R3Point& v) const {
        return !(*this < v);
    }
};

bool intersectPlanes(
    const R3Point& p0, const R3Vector& n0,
    const R3Point& p1, const R3Vector& n1,
    R3Point& p, R3Vector& v
);

bool intersectPlaneAndLine(
    const R3Point& p0, const R3Vector& n,
    const R3Point& p1, const R3Vector& v,
    R3Point& p
);

inline std::istream& operator>>(std::istream& str, R3Vector& v) {
    str >> v.x >> v.y >> v.z;
    return str;
}

inline std::ostream& operator<<(std::ostream& str, const R3Vector& v) {
    str << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    return str;
}

inline std::istream& operator>>(std::istream& str, R3Point& p) {
    str >> p.x >> p.y >> p.z;
    return str;
}

inline std::ostream& operator<<(std::ostream& str, const R3Point& p) {
    str << '(' << p.x << ", " << p.y << ", " << p.z << ')';
    return str;
}

} // end of namespace R3Graph

#endif
