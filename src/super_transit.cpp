#include <memory.h>
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

static void convert_transit_info(transit_info info) {
    node_database db(info, 0min, 0);
    //convert info into database where
    //   * nodes are interconnected
    //       - to next

    //   * can ask db for heuristic from node to finish
    //   * can ask db for start's next closest neighbor
    //   * can ask db for start's current closest neighbor's distance (db keeps track of this)
}

std::vector<stransit::trip> get_trips(stransit::trip_options options) {
    return std::vector<stransit::trip>();
}
