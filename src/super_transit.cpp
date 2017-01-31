#include <memory>
#include <queue>
#include <map>
#include <iostream>
#include <json.hpp>

#include "super_transit.hpp"
#include "node_database.hpp"

using namespace stransit;
using namespace std::literals;

using json = nlohmann::json;

struct node_wrapper {
    node_wrapper(node* ptr_, std::chrono::minutes time_): ptr(ptr_), total_time(time_) {}
    node* ptr;
    std::chrono::minutes total_time;
};

class node_priority_compare {
public:
    bool operator() (const node_wrapper& a, const node_wrapper& b) {
        return a.total_time > b.total_time;
    }
};

using node_priority_queue = std::priority_queue<node_wrapper, std::vector<node_wrapper>,
                                                node_priority_compare>;

/**
 * Performs search algorithm
 */
static void perform_search(node_database& db) {

    node_priority_queue pq;
    pq.emplace(&db.starting_node(), db.starting_node().time());

    while (!pq.empty() && !db.done_searching()) {
        node* n = pq.top().ptr;
        pq.pop();

        for (auto& n_edge: n->neighbors()) {
            std::chrono::minutes new_time = n->time() + n_edge.time_interval;
            optional_edge prev = {n_edge.time_interval, n};
            node& neighbor = n_edge.neighbor;
            if (!neighbor.visited) {
                neighbor.visited = true;
                neighbor.set_time(new_time);
                neighbor.set_previous(prev);
                pq.emplace(&neighbor, neighbor.time() + db.heuristic(neighbor));
            } else if (new_time < neighbor.time()) {
                neighbor.set_time(new_time);
                neighbor.set_previous(prev);
            }
        }

    }
}

//TODO fix the exceptions

std::map<std::string, geo_coords> make_stop_locations(const std::string& stop_locations) {
    try {
        std::map<std::string, geo_coords> result;
        json j = json::parse(stop_locations);
        if (!j.is_array()) {
            throw "stop_locations json not array.";
        }

        for (auto& elem : j) {
            std::string name = elem["Name"].get<std::string>();
            double lon = elem["Longitude"].get<double>();
            double lat = elem["Latitude"].get<double>();
            result[name] = geo_coords(lon, lat);
        }

        return result;
    } catch (...) {
        throw "Error parsing";
    }
}

/**Throws*/
time_hm parse_time(std::string str) {
    std::string::size_type colon_pos = str.find(":");
    std::string hour_string = str.substr(0, colon_pos);
    std::string min_string =  str.substr(colon_pos + 1, 2);
    std::string meridian_string = str.substr(colon_pos + 3, 2);

    int hour, minute;
    try {
        hour = std::stoi(hour_string);
        minute = std::stoi(min_string);
    } catch (...) {
        throw "error parsing hour and minute";
    }

    //check bounds
    if (hour < 0 || hour > 12 || minute < 0 || minute >= 60) {
        throw "out of bounds time";
    }

    if (meridian_string == "AM") {
        if (hour == 12) {
            hour = 0;
        }
    } else if (meridian_string == "PM") {
        if (hour != 12) {
            hour += 12;
        }
    } else {
        throw "bad meridian";
    }

    return {std::chrono::hours(hour), std::chrono::minutes(minute)};
}

using schedules_iterator = std::vector<stop_info_schedule>::iterator;

schedules_iterator find_matching_schedule(schedules_iterator begin, schedules_iterator end,
                                          const stop_info& stop) {
    return std::find_if(begin, end,
                        [&stop](const stop_info_schedule& schedule) -> bool {
                            return schedule.route_number == stop.route_number &&
                                schedule.day.overlap(stop.day) &&
                                schedule.direction == stop.direction;
                        });
}

void transit_info::add_stop(const stop_info& stop) {
    schedules_iterator itr;
    itr = find_matching_schedule(stop_schedules.begin(),
                                 stop_schedules.end(), stop);

    if (itr == stop_schedules.end()) {
        stop_info_schedule new_schedule = {
            .route_number = stop.route_number,
            .day = stop.day,
            .direction = stop.direction,
            .stops = {}
        };
        stop_schedules.push_back(new_schedule);
        return;
    }

    while (itr < stop_schedules.end()) {
        itr->stops.push_back(stop);
        itr = find_matching_schedule(++itr,
                                     stop_schedules.end(), stop);

    }
}

transit_info stransit::generate_transit_info_from_json(const std::string& stop_schedule,
                                                       const std::string& stop_locations) {
    std::map<std::string, geo_coords> locations = make_stop_locations(stop_locations);
    transit_info result;

    json j = json::parse(stop_schedule);
    if (!j.is_array()) {
        throw "stop_schedule json not array.";
    }

    for (json& elem : j) {
        if (!( elem["Stop"].is_string() &&
               elem["Route"].is_number() &&
               elem["Day"].is_string() &&
               elem["Direction"].is_string() &&
               elem["Time"].is_string() ))
        {
            continue;
        }

        std::string name = elem["Stop"];
        int route = elem["Route"];
        std::string day_str = elem["Day"];
        std::string direction = elem["Direction"];
        std::string time_str = elem["Time"];
        time_hm t = parse_time(time_str);
        geo_coords loc = locations.at(name);
        day_set day = day_set(day_str);

        stop_info stop = {
            .name = name,
            .route_number = route,
            .day = day,
            .direction = direction,
            .time_of_stop = t,
            .location = loc
        };

        result.add_stop(stop);

    }

    return result;
}

std::vector<stransit::trip> stransit::get_trips(stransit::trip_options options) {
    node_database db(options.info, options.max_wait_time, options.max_walk_dist,
                     options.start_position, options.end_position,
                     options.start_day, options.start_time, options.max_num_walking_stops);

    perform_search(db);

    std::vector<node*> solved = db.solved_nodes();
    std::vector<stransit::trip> result_trips;

    //make trips from solved nodes
    for (node* n : solved) {
        trip result;
        miles dist_walk_to_destination = n->location().distance_to(options.end_position);
        std::chrono::minutes time_walk_to_destination = walking_time(dist_walk_to_destination);
        time_hm time_of_arrival = n->time_of_stop().add(time_walk_to_destination);

        result.duration_of_travel = n->time() + time_walk_to_destination;

        result.waypoints.push_back({
                .name = "Destination",
                    .time = time_of_arrival,
                    .point_kind = trip::trip_waypoint::waypoint_finish,
                    .path_to_next = {0min, miles(0), ""}
            });


        trip::trip_waypoint first_point = {
            .name = n->name(),
            .time = n->time_of_stop(),
            .point_kind = trip::trip_waypoint::waypoint_stop,
            .path_to_next = {
                .period_of_travel = time_walk_to_destination,
                .length_of_travel = dist_walk_to_destination,
                .description = "TODO modify to provide description. ie take bus 2"
            }
        };

        //std move
        result.waypoints.insert(result.waypoints.begin(), first_point);

        optional_edge current_edge = n->previous();
        while (current_edge.neighbor != nullptr) {
            trip::trip_waypoint point = {
                .name = current_edge.neighbor->name(),
                .time = current_edge.neighbor->time_of_stop(),
                .point_kind = trip::trip_waypoint::waypoint_stop,
                .path_to_next = {
                    .period_of_travel = current_edge.time_interval,
                    .length_of_travel = walking_distance_for_time(current_edge.time_interval),
                    .description = "TODO modify to provide description. ie take bus 2"
                }
            };

            //std move
            result.waypoints.insert(result.waypoints.begin(), point);
            current_edge = current_edge.neighbor->previous();
        }

        result_trips.push_back(result);
    }

    return result_trips;
}
