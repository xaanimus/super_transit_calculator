#ifndef START_NODE_H
#define START_NODE_H

#include "node.hpp"

class start_node: public node {
public:
    start_node(day_type current_day, time_hm current_time,
               geo_coords location);

    std::string name() const;
    int route_number() const;
    day_set day() const;
    std::string direction() const;
    time_hm time_of_stop() const;
    edge_storage& neighbors();
    const geo_coords& location() const;

    const optional_edge& previous() const;
    void set_previous(const optional_edge& previous_node);
    miles distance() const;
    void set_distance(miles dist);

private:
    day_type m_current_day;
    time_hm m_current_time;
    edge_storage m_neighbors;
    geo_coords m_location;

    optional_edge m_previous;
};


#endif /* START_NODE_H */
