#ifndef UNITS_COORDS_H
#define UNITS_COORDS_H

#include <limits>
#include <chrono>

namespace stransit {
    using degrees = double;
    using miles = double;

    namespace literals {
        miles operator "" _mi(long double value);
    }

    struct vec2 {
        double x, y;
    };

    struct geo_coords {
        degrees longitude, latitude;

        geo_coords(degrees lon, degrees lat) : longitude(lon), latitude(lat) {}
        geo_coords() : longitude(), latitude() {}
        miles distance_to(const geo_coords& other) const;

        /**returns self as vec2, using x = longitude and y = latitude*/
        vec2& as_vec2();
        vec2 copy_as_vec2() const;
    };

    degrees delta_geo_angle(miles distance);

    std::chrono::minutes walking_time(miles dist);
    miles walking_distance_for_time(std::chrono::minutes t);
};

#endif /* UNITS_COORDS_H */
