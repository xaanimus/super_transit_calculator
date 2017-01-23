#ifndef UNITS_COORDS_H
#define UNITS_COORDS_H

namespace stransit {
    using degrees = double;
    using miles = double;

    struct geo_coords {
        degrees longitude, latitude;
        miles distance_to(const geo_coords& other) const;
    };
}

#endif /* UNITS_COORDS_H */
