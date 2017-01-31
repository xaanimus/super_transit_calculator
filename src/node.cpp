#include "node.hpp"
#include <algorithm>

node::~node() {}

vec2 node::get_node_position::operator()(node* n) {
    return n->location().copy_as_vec2();
}

stop_node::stop_node(std::string name_, int route_number_, day_set day_,
                     std::string direction_, time_hm time_of_stop_, edge_storage neighbors_,
                     optional_edge previous_, std::chrono::minutes duration_, geo_coords location_)
    : m_name(name_), m_route_num(route_number_), m_day(day_), m_direction(direction_),
      m_time_of_stop(time_of_stop_), m_neighbors(neighbors_), m_location(location_),
      m_previous(previous_), m_duration(duration_) {}

std::string stop_node::name() const {
    return m_name;
}

int stop_node::route_number() const {
    return m_route_num;
}

day_set stop_node::day() const {
    return m_day;
}

std::string stop_node::direction() const {
    return m_direction;
}

time_hm stop_node::time_of_stop() const {
    return m_time_of_stop;
}

edge_storage& stop_node::neighbors() {
    return m_neighbors;
}

const geo_coords& stop_node::location() const {
    return m_location;
}

const optional_edge& stop_node::previous() const {
    return m_previous;
}

void stop_node::set_previous(const optional_edge& previous_node) {
    m_previous = previous_node;
}

void stop_node::add_neighbor(const edge& neighbor) {
    m_neighbors.insert(neighbor);
}

std::chrono::minutes stop_node::time() const {
    return m_duration;
}

void stop_node::set_time(std::chrono::minutes new_time) {
    m_duration = new_time;
}
