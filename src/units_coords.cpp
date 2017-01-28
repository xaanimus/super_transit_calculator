#include "units_coords.hpp"
#include "math.h"

using namespace stransit;
using radians = double;

static const miles EARTH_RADIUS_MILES = 3959;
const double WALKING_TIME_MINUTES_PER_MILE = 30;

#define SQR(fn, expr) (pow(fn(expr), 2))

static inline radians deg_to_rad(degrees deg) {
    return deg / 180 * M_PI;
}

static inline degrees rad_to_deg(radians rad) {
    return rad / M_PI * 180;
}


miles stransit::literals::operator "" _mi(long double value) {
    return miles(value);
}

miles geo_coords::distance_to(const stransit::geo_coords &other) const {
    radians delta_lon = deg_to_rad(other.longitude - longitude);
    radians delta_lat = deg_to_rad(other.latitude - latitude);
    degrees a = SQR(sin, delta_lat / 2) + cos(latitude) * cos(other.latitude)
        * SQR(sin, delta_lon/2);
    degrees c = 2 * atan2(sqrt(a), sqrt(1-a));
    miles distance = EARTH_RADIUS_MILES * c;
    return distance;
}

degrees stransit::delta_geo_angle(miles distance) {
    return rad_to_deg(distance / EARTH_RADIUS_MILES);
}

vec2& geo_coords::as_vec2() {
    return *((vec2*)this);
}

vec2 geo_coords::copy_as_vec2() const {
    return *((vec2*)this);
}


std::chrono::minutes stransit::walking_time(miles dist) {
    int time_in_minutes = (int) ceil(WALKING_TIME_MINUTES_PER_MILE * dist);
    return std::chrono::minutes(time_in_minutes);
}

miles stransit::walking_distance_for_time(std::chrono::minutes t) {
    return t.count() / WALKING_TIME_MINUTES_PER_MILE;
}
