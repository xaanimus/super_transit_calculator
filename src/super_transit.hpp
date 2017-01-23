#ifndef SUPER_TRANSIT_H
#define SUPER_TRANSIT_H

#include <string>
#include <vector>
#include "transit_time.hpp"
#include "units_coords.hpp"

namespace stransit {

    struct trip {
    };

    struct stop_info {
        std::string name;
        int route_number;
        std::string day;
        std::string direction;
        time_hm time_of_stop;
        geo_coords location;
    };

    /**
     * a schedule that contains stops for a certain route number and set of days and a direction
     * INVARIANT: this->stops must be sorted in order of time (stops[n].time_of_stop < stops[n+1].time_of_stop)
     * ^^^ the invariant above must be true before using this struct, or you will encounter issues
     */
    struct stop_info_schedule {
        int route_number;
        std::string day;
        std::string direction;
        std::vector<stop_info> stops;
    };

    struct transit_info {
        /** @Brief stores schedules for stops.
         * Grouped by route_number, day, direction
         */
        std::vector<stop_info_schedule> stop_schedules;
    };

    struct trip_options {
        geo_coords start_position;
        time_hm start_time;
        transit_info info;
        geo_coords end_position;
        miles end_radius;
    };

    std::vector<trip> get_trips(trip_options options);
};


#endif /* SUPER_TRANSIT_H */
