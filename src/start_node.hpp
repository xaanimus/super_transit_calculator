#ifndef START_NODE_H
#define START_NODE_H

#include "node.hpp"

const std::string DAY_ANY;

class start_node: public node {
public:
    start_node(std::string current_day, time_hm current_time,
               geo_coords location);

    std::string name() const;
    int route_number() const;
    std::string day() const;
    std::string direction() const;
    time_hm time_of_stop() const;
    std::vector<edge>& neighbors();
    const geo_coords& location() const;

    node* previous() const;
    void set_previous(node* previous_node);
    miles distance() const;
    void set_distance(miles dist);

private:
    std::string m_current_day;
    time_hm m_current_time;
    std::vector<edge> m_neighbors;
    geo_coords m_location;

    node* m_previous;
};


#endif /* START_NODE_H */
