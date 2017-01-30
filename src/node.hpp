#ifndef NODE_H
#define NODE_H

#include "super_transit.hpp"
#include <memory.h>
#include <unordered_set>
#include <set>
#include <iostream>

using namespace stransit;
using namespace std::literals;

class node;
struct edge;
struct optional_edge;

/**
 * unidirected edge between two nodes
 */
struct edge {
    std::chrono::minutes time_interval;
    node& neighbor;
};

struct edge_hash {
    size_t operator() (const edge& e) {
        std::uintptr_t num = (std::uintptr_t)&e.neighbor;
        auto hash = std::hash<std::uintptr_t>();
        size_t h = hash(num);
        return h;
    }
};

struct edge_less {
    bool operator () (const edge& lhs, const edge& rhs) {
        return &lhs.neighbor < &rhs.neighbor;
    }
};

struct edge_equal_to {
    bool operator() (const edge& a, const edge& b) {
        return &a.neighbor == &b.neighbor;
    }
};

//using edge_storage = std::unordered_set<edge, edge_hash, edge_equal_to>;
using edge_storage = std::set<edge, edge_less>;

class node {
public:
    virtual ~node();

    virtual std::string name() const = 0;
    virtual int route_number() const = 0;
    virtual std::string day() const = 0;
    virtual std::string direction() const = 0;
    virtual time_hm time_of_stop() const = 0;
    virtual edge_storage& neighbors() = 0;
    virtual const geo_coords& location() const = 0;

    virtual const optional_edge& previous() const = 0;
    virtual void set_previous(const optional_edge& previous_node) = 0;
    virtual miles distance() const = 0;
    virtual void set_distance(miles dist) = 0;

    std::chrono::minutes time();
    void set_time(std::chrono::minutes new_time);

    bool visited = false;

    struct get_node_position {
        vec2 operator()(node* n);
    };
};


struct optional_edge {
    std::chrono::minutes time_interval;
    node* neighbor;

};
const optional_edge optional_edge_nil = {0min, nullptr};

class stop_node : public node {
public:
    stop_node(std::string name_, int route_number_, std::string day_,
              std::string direction_, time_hm time_of_stop_, edge_storage neighbors,
              optional_edge previous_, miles distance_, geo_coords location_);

    std::string name() const override;
    int route_number() const override;
    std::string day() const override;
    std::string direction() const override;
    time_hm time_of_stop() const override;
    edge_storage& neighbors() override;
    const geo_coords& location() const override;

    const optional_edge& previous() const override;
    void set_previous(const optional_edge& previous_node) override;
    miles distance() const override;
    void set_distance(miles dist) override;
    /// adds neighbor without repetition.
    void add_neighbor(const edge& neighbor);

private:
    std::string m_name;
    int m_route_num;
    std::string m_day;
    std::string m_direction;
    time_hm m_time_of_stop;
    edge_storage m_neighbors;
    geo_coords m_location;

    optional_edge m_previous;
    miles m_distance;
};


#endif /* NODE_H */
