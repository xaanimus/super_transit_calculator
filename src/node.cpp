#include "node.hpp"
#include <algorithm>

node::~node() {}

std::chrono::minutes node::time() {
    return walking_time(this->distance());
}

void node::set_time(std::chrono::minutes new_time) {
    this->set_distance(walking_distance_for_time(new_time));
}

stop_node::stop_node(std::string name_, int route_number_, std::string day_,
                     std::string direction_, time_hm time_of_stop_, std::vector<edge> neighbors_,
                     node* previous_, miles distance_, geo_coords location_)
    : m_name(name_), m_route_num(route_number_), m_day(day_), m_direction(direction_),
      m_time_of_stop(time_of_stop_), m_neighbors(neighbors_), m_location(location_),
      m_previous(previous_), m_distance(distance_) {}

std::string stop_node::name() const {
    return m_name;
}

int stop_node::route_number() const {
    return m_route_num;
}

std::string stop_node::day() const {
    return m_day;
}

std::string stop_node::direction() const {
    return m_direction;
}

time_hm stop_node::time_of_stop() const {
    return m_time_of_stop;
}

std::vector<edge>& stop_node::neighbors() {
    return m_neighbors;
}

const geo_coords& stop_node::location() const {
    return m_location;
}

node* stop_node::previous() const {
    return m_previous;
}

void stop_node::set_previous(node *previous_node) {
    m_previous = previous_node;
}

miles stop_node::distance() const {
    return m_distance;
}

void stop_node::set_distance(miles dist) {
    m_distance = dist;
}

void stop_node::add_neighbor(edge neighbor) {
    m_neighbors.push_back(neighbor);
}

void stop_node::add_neighbor_no_repeat(edge neighbor) {
    auto result = std::find_if(m_neighbors.begin(), m_neighbors.end(),
                               [&neighbor](edge each_neighbor){
            return &each_neighbor == &neighbor;
        });

    if (result == m_neighbors.end()) {
        m_neighbors.push_back(neighbor);
    }
}
