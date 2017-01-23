#include "units_coords.hpp"
#include "math.h"

using namespace stransit;
using radians = double;

static const miles EARTH_RADIUS_MILES = 3959;

static double sin2(double x) {
    return pow(sin(x), 2);
}

static radians deg_to_rad(degrees deg) {
    return deg / 180 * M_PI;
}

miles geo_coords::distance_to(const stransit::geo_coords &other) const {
    degrees delta_lon = deg_to_rad(other.longitude - longitude);
    degrees delta_lat = deg_to_rad(other.latitude - latitude);
    degrees a = sin2(delta_lat / 2) + cos(latitude) * cos(other.latitude) * sin2(delta_lon/2);
    degrees c = 2 * atan2(sqrt(a), sqrt(1-a));
    miles distance = EARTH_RADIUS_MILES * c;
    return distance;
}
