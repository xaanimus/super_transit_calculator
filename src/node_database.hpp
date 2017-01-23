#include "node.hpp"

/**
   Stores nodes to be used by the search algorithm
   Not thread safe
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
                  miles max_walking_distance);
    ~node_database();

    node_storage& nodes();

    ///add this schedule to the nodes
    void add_schedule(const stop_info_schedule& schedule);

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

    node_storage m_nodes;
    std::chrono::minutes m_max_wait_time;
    miles m_max_walking_distance;
};


