#include <iostream>
#include <math.h>
#include "node_database.hpp"

using namespace std::literals;
using node_storage = node_database::node_storage;

/** TODO improve
 * checks if days a and b match
 */
bool check_days_match(std::string a, std::string b) {
    return a == b;
}

node_database::node_database(transit_info info, std::chrono::minutes max_wait_time,
                             miles max_walking_distance, geo_coords start_point,
                             geo_coords end_point, std::string starting_day,
                             time_hm starting_time)
    : m_max_wait_time(max_wait_time), m_max_walking_distance(max_walking_distance),
      m_start_point(start_point), m_end_point(end_point),
      m_starting_node(starting_day, starting_time, start_point)
{
    //add all schedules
    for (auto& schedule : info.stop_schedules) {
        add_schedule(schedule);
    }

    connect_nodes_from_stops();
    connect_nodes_by_walking();
    connect_starting_nodes();
    add_final_nodes();
}

node_database::~node_database() {
    for(node* n : m_nodes) {
        delete n;
    }
}

node_database::node_storage& node_database::nodes() {
    return m_nodes;
}

std::chrono::minutes node_database::heuristic(const node &n) const {
    return walking_time( n.location().distance_to(m_end_point) );
}

///add this schedule to the nodes
void node_database::add_schedule(const stop_info_schedule& schedule) {
    stop_node* previous_node = nullptr;
    for (auto& stop : schedule.stops) {
        stop_node* new_node = new stop_node(stop.name, schedule.route_number, schedule.day,
                                            stop.direction, stop.time_of_stop,
                                            std::vector<edge>(), optional_edge_nil, 0,
                                            stop.location );
        m_nodes.push_back(new_node);
        if (previous_node != nullptr) {
            std::chrono::minutes diff =
                previous_node->time_of_stop().difference_to(new_node->time_of_stop());
            previous_node->add_neighbor((edge){diff, *new_node});
        }
        previous_node = new_node;
    }
}

// consider optimizing this. current runtime is O(N^2)
// where N == m_nodes.size()
void node_database::connect_nodes_from_stops() {
    //iter thru nodes
    //    look up stop for node
    //    for each next stop' from stop, add stop' to node.neighbors

    for (node* n: m_nodes) {
        std::string stop_name = n->name();

        //iter thru m_nodes that have same name as n
        //same day
        for (node* other_node: m_nodes) {
            std::chrono::minutes wait_time =
                n->time_of_stop().difference_to(other_node->time_of_stop());
            bool too_much_waiting = wait_time > m_max_wait_time;
            bool days_match = check_days_match(n->day(), other_node->day());

            if (other_node->name() != stop_name ||
                other_node == n || !days_match ||
                too_much_waiting )
            {
                continue;
            }

            for (const edge& neighbor: other_node->neighbors()) {
                //TODO use correct diff
                edge new_edge = {wait_time + neighbor.time_interval, neighbor.neighbor};
                static_cast<stop_node*>(n)->add_neighbor_no_repeat(new_edge);
            }
        }
    }
}

void node_database::connect_nodes_by_walking() {
    for (node* n: m_nodes) {
        for(node* other: m_nodes) {
            miles distance = n->location().distance_to(other->location());
            bool days_match = check_days_match(n->day(), other->day());
            if (n == other || distance > m_max_walking_distance || !days_match)
            {
                continue;
            }
            edge new_edge = {walking_time(distance), *other};
            static_cast<stop_node*>(n)->add_neighbor_no_repeat(new_edge);
        }
    }
}

void node_database::connect_starting_nodes() {
    for (node* n: m_nodes) {
        miles dist = n->location().distance_to(m_starting_node.location());
        if (dist > m_max_walking_distance) {
            continue;
        } else {
            edge new_neighbor = {walking_time(dist), *n};
            m_starting_node.neighbors().push_back(new_neighbor);
        }
    }
}

void node_database::add_final_nodes() {
    for (node* n: m_nodes) {
        miles dist = m_end_point.distance_to(n->location());
        if (dist > m_max_walking_distance) {
            continue;
        } else {
            m_final_nodes.push_back(n);
        }
    }
}

bool node_database::done_searching() {

    //remove all visited nodes from m_final_nodes
    for (auto itr = m_final_nodes.begin(); itr < m_final_nodes.end(); ++itr) {
        if ((*itr)->visited) {
            m_solved_nodes.push_back(*itr);
            itr = m_final_nodes.erase(itr);
        }
    }

    return m_final_nodes.empty();
}

start_node& node_database::starting_node() {
    return m_starting_node;
}

const node_storage& node_database::solved_nodes() const {
    return m_solved_nodes;
}
