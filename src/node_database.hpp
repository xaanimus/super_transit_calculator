#include "node.hpp"
#include "start_node.hpp"

/**
 * Stores nodes to be used by the search algorithm.
 * Not thread safe.
 * All nodes are initialized with a distance of infinity, except for
 * the start node, which is initialized with a distance of 0
 */
class node_database {
public:
    using node_storage = std::vector<node*>;

    /**
     * Initializes a node database. Connects stops (makes neighbors) in the following manner:
     * 1. Each stop is connected to the next stop in its schedule if it exists
     * 2. 
     */
    node_database(transit_info info, std::chrono::minutes max_wait_time,
                  miles max_walking_distance, geo_coords start_point, geo_coords end_point,
                  std::string starting_day, time_hm starting_time);
    ~node_database();

    node_storage& nodes();

    ///add this schedule to the nodes
    void add_schedule(const stop_info_schedule& schedule);

    /**
     * Computes heuristic distance between a node and the end point.
     * The heuristic is the direct distance in miles between the node and the end point.
     */
    std::chrono::minutes heuristic(const node& n) const;

    start_node& starting_node();
    bool done_searching();
    const node_storage& solved_nodes() const;

private:
    /**
     * PRECONDITION: all nodes in m_nodes are ()stop_node*)
     * Connect nodes to all other reachable nodes from same stop
     */
    void connect_nodes_from_stops();

    /**
     * Connect nodes to all other nodes within walking distance
     */
    void connect_nodes_by_walking();

    /**
     * Connect starting point node to nodes within walking distance
     */
    void connect_starting_nodes();

    void add_final_nodes();

    node_storage m_nodes;
    std::chrono::minutes m_max_wait_time;
    miles m_max_walking_distance;
    geo_coords m_start_point, m_end_point;
    start_node m_starting_node;
    node_storage m_final_nodes;
    node_storage m_solved_nodes;
};

