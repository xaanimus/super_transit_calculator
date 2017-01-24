#include <memory>
#include <queue>
#include "super_transit.hpp"
#include "node_database.hpp"

using namespace stransit;
using namespace std::literals;


struct starting_position {

};

class trip_searcher {
public:
    void search();
};

void trip_searcher::search() {
}

static void convert_transit_info(transit_info info, std::chrono::minutes max_wait_time,
                                 miles max_walk_dist)
{
    //node_database db(info, max_wait_time, max_walk_dist);
    //convert info into database where
    //   * nodes are interconnected
    //       - to next

    //   * can ask db for heuristic from node to finish
    //   * can ask db for start's next closest neighbor
    //   * can ask db for start's current closest neighbor's distance (db keeps track of this)
}

struct node_wrapper {
    node_wrapper(node* ptr_, std::chrono::minutes time_): ptr(ptr_), total_time(time_) {}
    node* ptr;
    std::chrono::minutes total_time;
};

class node_priority_compare {
public:
    bool operator() (const node_wrapper& a, const node_wrapper& b) {
        return a.total_time > b.total_time;
    }
};

using node_priority_queue = std::priority_queue<node_wrapper, std::vector<node_wrapper>,
                                                node_priority_compare>;

/**
 * Performs search algorithm
 */
static void perform_search(node_database& db) {
    node_priority_queue pq;
    pq.emplace(&db.starting_node(), db.starting_node().time());

    while (!pq.empty()) {
        node* n = pq.top().ptr;
        pq.pop();

        for (auto& n_edge: n->neighbors()) {
            std::chrono::minutes new_time = n->time() + n_edge.time_interval;
            node& neighbor = n_edge.neighbor;
            if (!neighbor.visited) {
                neighbor.visited = true;
                neighbor.set_time(new_time);
                neighbor.set_previous(n);
                pq.emplace(&neighbor, neighbor.time() + db.heuristic(neighbor));
            } else if (new_time < neighbor.time()) {
                neighbor.set_time(new_time);
                neighbor.set_previous(n);
            }
        }
    }
}

std::vector<stransit::trip> get_trips(stransit::trip_options options) {
    node_database db(options.info, options.max_wait_time, options.max_walk_dist,
                     options.start_position, options.end_position,
                     options.start_day, options.start_time);

    perform_search(db);

    return std::vector<stransit::trip>();
}
