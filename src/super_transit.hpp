#ifndef SUPER_TRANSIT_H
#define SUPER_TRANSIT_H

#include <string>
#include <vector>
#include "transit_time.hpp"
#include "units_coords.hpp"
#include "day_set.hpp"

namespace stransit {

    struct trip {

        struct trip_edge {
            std::chrono::minutes period_of_travel;
            miles length_of_travel;
            std::string description;
        };

        struct trip_waypoint {
            enum kind {
                waypoint_begin,
                waypoint_stop,
                waypoint_finish,
            };

            std::string name;
            time_hm time;
            kind point_kind;
            trip_edge path_to_next;
        };

        std::vector<trip_waypoint> waypoints;
        std::chrono::minutes duration_of_travel;
    };

    struct stop_info {
        std::string name;
        int route_number;
        day_set day;
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
        day_set day;
        std::string direction;
        std::vector<stop_info> stops;
    };

    struct transit_info {
        /** @Brief stores schedules for stops.
         * Grouped by route_number, day, direction
         */
        std::vector<stop_info_schedule> stop_schedules;
        /** Adds stop into schedule, in order
         */
        void add_stop(const stop_info& stop);
    };

    transit_info generate_transit_info_from_json(const std::string& route_schedule,
                                                 const std::string& stop_locations);

    struct trip_options {
        geo_coords start_position;
        time_hm start_time;
        day_type start_day;
        transit_info info;
        geo_coords end_position;

        std::chrono::minutes max_wait_time;
        miles max_walk_dist;

        int max_num_walking_stops;
    };

    std::vector<trip> get_trips(trip_options options);
};


#endif /* SUPER_TRANSIT_H */
