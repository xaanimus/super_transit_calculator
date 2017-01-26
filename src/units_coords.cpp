#include "units_coords.hpp"
#include "math.h"

using namespace stransit;
using radians = double;

static const miles EARTH_RADIUS_MILES = 3959;
const double WALKING_TIME_MINUTES_PER_MILE = 30;

static double sin2(double x) {
    return pow(sin(x), 2);
}

static radians deg_to_rad(degrees deg) {
    return deg / 180 * M_PI;
}

miles stransit::literals::operator "" _mi(long double value) {
    return miles(value);
}

miles geo_coords::distance_to(const stransit::geo_coords &other) const {
    degrees delta_lon = deg_to_rad(other.longitude - longitude);
    degrees delta_lat = deg_to_rad(other.latitude - latitude);
    degrees a = sin2(delta_lat / 2) + cos(latitude) * cos(other.latitude) * sin2(delta_lon/2);
    degrees c = 2 * atan2(sqrt(a), sqrt(1-a));
    miles distance = EARTH_RADIUS_MILES * c;
    return distance;
}

std::chrono::minutes stransit::walking_time(miles dist) {
    int time_in_minutes = (int) ceil(WALKING_TIME_MINUTES_PER_MILE * dist);
    return std::chrono::minutes(time_in_minutes);
}

miles stransit::walking_distance_for_time(std::chrono::minutes t) {
    return t.count() / WALKING_TIME_MINUTES_PER_MILE;
}
