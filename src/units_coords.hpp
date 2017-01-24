#ifndef UNITS_COORDS_H
#define UNITS_COORDS_H

#include <limits>
#include <chrono>

namespace stransit {
    using degrees = double;
    using miles = double;

    struct geo_coords {
        degrees longitude, latitude;

        geo_coords(degrees lon, degrees lat) : longitude(lon), latitude(lat) {}
        geo_coords() : longitude(), latitude() {}
        miles distance_to(const geo_coords& other) const;
    };

    std::chrono::minutes walking_time(miles dist);
    miles walking_distance_for_time(std::chrono::minutes t);
};

#endif /* UNITS_COORDS_H */
