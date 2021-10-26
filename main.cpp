#include <iostream>
#include <cmath>
#include "R3Graph/R3Graph.h"

using namespace R3Graph;

const double EARTH_RADIUS = 6378000.0;
const double PI = atan(1) * 4;  //через арктангенс определяем pi atan(1)==pi/4

R3Vector radiusVector(double lat, double lon)
{
    double phi = lon * PI / 180.;  // Convert to radians
    double theta = lat * PI / 180.;
    double cosTheta = cos(theta);
    return R3Vector(cos(phi) * cosTheta, sin(phi) * cosTheta, sin(theta));
}

void earth2map(double mapLat, double mapLon, double lat, double lon, double &x, double &y)
{
    R3Vector ez(0., 0., 1.);
    R3Vector map_norm = radiusVector(mapLat, mapLon);
    R3Vector map_x = ez.vectorProduct(map_norm);
    map_x.normalize();
    R3Vector map_y = map_norm.vectorProduct(map_x);
    R3Point origin(0., 0., 0.);
    R3Point map_center = origin + map_norm * EARTH_RADIUS;
    R3Vector v = radiusVector(lat, lon);
    R3Point p;
    intersectPlaneAndLine(map_center, map_norm, origin, v, p);
    R3Vector v_map = p - map_center;
    x = v_map * map_x;
    y = v_map * map_y;
}

int main()
{
    while (true)
    {
        double mapLat, mapLon;
        std::cout << "Enter coordinates of the map center (lat,lon):" << std::endl;
        std::cin >> mapLat >> mapLon;
        if (!(std::cin.good()))
            break;
        while (true)
        {
            double lat, lon;
            std::cout << "Enter coordinates of point on the Earth surface:" << std::endl;
            std::cin >> lat >> lon;
            if ((!std::cin.good()))
                break;
            double x, y;
            earth2map(mapLat, mapLon, lat, lon, x, y);
            std::cout << "Coordinates on map: (" << x << ", " << y << ")" << std::endl;
        }
    }
    return 0;
}