#ifndef START_NODE_H
#define START_NODE_H

#include "node.hpp"

class start_node: public node {
public:
    start_node(day_type current_day, time_hm current_time,
               geo_coords location);

    std::string name() const override;
    int route_number() const override;
    day_set day() const override;
    std::string direction() const override;
    time_hm time_of_stop() const override;
    edge_storage& neighbors() override;
    const geo_coords& location() const override;

    const optional_edge& previous() const override;
    void set_previous(const optional_edge& previous_node) override;

    std::chrono::minutes time() const override;
    /** Because this is a start_node, this will have no effect */
    void set_time(std::chrono::minutes new_time) override;

private:
    day_type m_current_day;
    time_hm m_current_time;
    edge_storage m_neighbors;
    geo_coords m_location;

    optional_edge m_previous;
};


#endif /* START_NODE_H */
