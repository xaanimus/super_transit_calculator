#include "start_node.hpp"

start_node::start_node(day_type current_day, time_hm current_time, geo_coords location)
    : m_current_day(current_day), m_current_time(current_time), m_neighbors(),
      m_location(location), m_previous() {}

std::string start_node::name() const {
    return "__start_node";
}

int start_node::route_number() const {
    return -1;
}

day_set start_node::day() const {
    return m_current_day;
}

std::string start_node::direction() const {
    return "Starting_Direction";
}

time_hm start_node::time_of_stop() const {
    return m_current_time;
}

edge_storage& start_node::neighbors() {
    return m_neighbors;
}

const geo_coords& start_node::location() const {
    return m_location;
}

const optional_edge& start_node::previous() const {
    return m_previous;
}

void start_node::set_previous(const optional_edge& previous_node) {
    m_previous = previous_node;
}

std::chrono::minutes start_node::time() const {
    return 0min;
}

/** Because this is a start_node, this will have no effect */
void start_node::set_time(std::chrono::minutes) {
    return;
}
