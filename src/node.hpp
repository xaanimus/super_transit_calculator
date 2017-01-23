#ifndef NODE_H
#define NODE_H

#include "super_transit.hpp"
#include <memory.h>

using namespace stransit;

struct edge;

class node {
public:
    virtual ~node();

    virtual std::string name() const = 0;
    virtual int route_number() const = 0; //no
    virtual std::string day() const = 0;  //no
    virtual std::string direction() const = 0;
    virtual time_hm time_of_stop() const = 0;
    virtual std::vector<edge>& neighbors() = 0;
    virtual const geo_coords& location() const = 0;

    virtual node* previous() const = 0;
    virtual void set_previous(node* previous_node) = 0;
    virtual miles distance() const = 0;
    virtual void set_distance(miles dist) = 0;
};

/**
 * unidirected edge between two nodes
 */
struct edge {
    std::chrono::minutes time_interval;
    node& neighbor;
};



class stop_node : public node {
public:
    stop_node(std::string name_, int route_number_, std::string day_,
              std::string direction_, time_hm time_of_stop_, std::vector<edge> neighbors,
              node* previous_, miles distance_, geo_coords location_);

    std::string name() const override;
    int route_number() const override;
    std::string day() const override;
    std::string direction() const override;
    time_hm time_of_stop() const override;
    std::vector<edge>& neighbors() override;
    const geo_coords& location() const override;

    node* previous() const override;
    void set_previous(node* previous_node) override;
    miles distance() const override;
    void set_distance(miles dist) override;
    void add_neighbor(edge neighbor);
    /// same as add_neighbor, but will check that neighbor does not already exist
    void add_neighbor_no_repeat(edge neighbor);

private:
    std::string m_name;
    int m_route_num;
    std::string m_day;
    std::string m_direction;
    time_hm m_time_of_stop;
    std::vector<edge> m_neighbors;
    geo_coords m_location;

    node* m_previous;
    miles m_distance;
};


#endif /* NODE_H */
