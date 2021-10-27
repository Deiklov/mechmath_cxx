#include <iostream>
#include <cmath>
#include "R3Graph/R3Graph.h"

using namespace R3Graph;

const double EARTH_RADIUS = 6378000.0;
const double PI = atan(1) * 4;  // через арктангенс определяем pi, atan(1)==pi/4

auto radiusVector(double lat, double lon)
{
    double phi = lon * PI / 180.;
    double theta = lat * PI / 180.;
    double cosTheta = cos(theta);
    return R3Vector(cos(phi) * cosTheta, sin(phi) * cosTheta, sin(theta));
}

std::pair<double, double> earth2map(double mapLat, double mapLon, double lat, double lon)
{
    R3Vector ez(0., 0., 1.);
    R3Vector map_norm = radiusVector(mapLat, mapLon);
    R3Vector map_x = ez.vectorProduct(map_norm);
    map_x.normalize();
    R3Vector map_y = map_norm.vectorProduct(map_x);

    R3Point zero(0., 0., 0.);
    R3Point map_center = zero + map_norm * EARTH_RADIUS;
    R3Vector point_on_surface_vect = radiusVector(lat, lon);

    R3Point p;
    intersectPlaneAndLine(map_center, map_norm, zero, point_on_surface_vect, p);
    R3Vector v_map = p - map_center;
    double x = v_map * map_x;
    double y = v_map * map_y;
    return {x, y};
}

int main()
{
    while (true)
    {
        double mapLat, mapLon;
        std::cout << "enter coordinates of the map center (lat,lon):" << std::endl;
        std::cin >> mapLat >> mapLon;
        if (!(std::cin.good()))
            break;

        double lat, lon;
        std::cout << "enter coordinates of point on the Earth surface:" << std::endl;
        std::cin >> lat >> lon;
        if ((!std::cin.good()))
            break;

        auto [x, y] = earth2map(mapLat, mapLon, lat, lon);
        std::cout << "coordinates on map: (" << x << ", " << y << ")" << std::endl;
    }
    return 0;
}