#include <iostream>
#include <algorithm>
#include <catch.hpp>
#include "node_database.hpp"

using namespace std::literals;

TEST_CASE("Add Schedule") {
    // Checks that when a schedule is added, adjacent stops are accessible by using node::neighbors

    transit_info blank_transit_info;

    stop_info_schedule schedule = {
        .route_number = 1,
        .day = "Monday",
        .direction = "Outbound",
        .stops = {
            {"alpha", 1, "Monday", "Outbound", time_hm(2h, 30min), {}},
            {"bravo", 1, "Monday", "Outbound", time_hm(2h, 34min), {}},
            {"charlie", 1, "Monday", "Outbound", time_hm(2h, 40min), {}},
        }
    };

    node_database db(blank_transit_info, 1min);
    db.add_schedule(schedule);

    REQUIRE(db.nodes().size() == 3);

    node* alpha = db.nodes()[0],
        * bravo = db.nodes()[1],
        * charlie = db.nodes()[2];

    auto& a_neighbors = alpha->neighbors();
    auto a_itr_alpha = std::find_if(a_neighbors.begin(), a_neighbors.end(),
                                    [&alpha](auto& n) -> bool {
                                        return &(n.neighbor) == alpha;
                                    });
    auto a_itr_bravo = std::find_if(a_neighbors.begin(), a_neighbors.end(),
                                    [&bravo](auto& n) -> bool {
                                        return &(n.neighbor) == bravo;
                                    });
    auto a_itr_charlie = std::find_if(a_neighbors.begin(), a_neighbors.end(),
                                      [&charlie](auto& n) -> bool {
                                          return &(n.neighbor) == charlie;
                                      });
    REQUIRE(a_itr_alpha == a_neighbors.end());
    REQUIRE(a_itr_bravo != a_neighbors.end());
    REQUIRE(a_itr_charlie == a_neighbors.end());

    auto& b_neighbors = bravo->neighbors();
    auto b_itr_alpha = std::find_if(b_neighbors.begin(), b_neighbors.end(),
                                    [&alpha](auto& n) -> bool {
                                        return &(n.neighbor) == alpha;
                                    });
    auto b_itr_bravo = std::find_if(b_neighbors.begin(), b_neighbors.end(),
                                    [&bravo](auto& n) -> bool {
                                        return &(n.neighbor) == bravo;
                                    });
    auto b_itr_charlie = std::find_if(b_neighbors.begin(), b_neighbors.end(),
                                      [&charlie](auto& n) -> bool {
                                          return &(n.neighbor) == charlie;
                                      });
    REQUIRE(b_itr_alpha == b_neighbors.end());
    REQUIRE(b_itr_bravo == b_neighbors.end());
    REQUIRE(b_itr_charlie != b_neighbors.end());

    auto& c_neighbors = charlie->neighbors();
    auto c_itr_alpha = std::find_if(c_neighbors.begin(), c_neighbors.end(),
                                    [&alpha](auto& n) -> bool {
                                        return &(n.neighbor) == alpha;
                                    });
    auto c_itr_bravo = std::find_if(c_neighbors.begin(), c_neighbors.end(),
                                    [&bravo](auto& n) -> bool {
                                        return &(n.neighbor) == bravo;
                                    });
    auto c_itr_charlie = std::find_if(c_neighbors.begin(), c_neighbors.end(),
                                      [&charlie](auto& n) -> bool {
                                          return &(n.neighbor) == charlie;
                                      });
    REQUIRE(c_itr_alpha == c_neighbors.end());
    REQUIRE(c_itr_bravo == c_neighbors.end());
    REQUIRE(c_itr_charlie == c_neighbors.end());
}

TEST_CASE("connect nodes") {

    //both routes go through stop "bravo"
    stop_info_schedule schedule_1 = {
        .route_number = 1,
        .day = "Monday",
        .direction = "Outbound",
        .stops = {
            {"alpha", 1, "Monday", "Outbound", time_hm(2h, 30min), {}},
            {"bravo", 1, "Monday", "Outbound", time_hm(2h, 34min), {}},
            {"charlie", 1, "Monday", "Outbound", time_hm(2h, 40min), {}},
        }
    };
    stop_info_schedule schedule_2 = {
        .route_number = 2,
        .day = "Monday",
        .direction = "Outbound",
        .stops = {
            {"delta", 1, "Monday", "Outbound", time_hm(2h, 30min), {}},
            {"bravo", 1, "Monday", "Outbound", time_hm(2h, 50min), {}},
            {"echo", 1, "Monday", "Outbound", time_hm(3h, 40min), {}},
        }
    };

    transit_info info = {{schedule_1, schedule_2}};

    node_database db(info, 50min);

    node_database::node_storage& nodes = db.nodes();

    auto bravo_1 = std::find_if(nodes.begin(), nodes.end(), [](node* n) -> bool {
            return n->route_number() == 1 && n->name() == "bravo";
        });

    REQUIRE(bravo_1 != nodes.end());

    std::vector<edge>& bravo_neighbors = (*bravo_1)->neighbors();
    auto charlie_itr = std::find_if(bravo_neighbors.begin(), bravo_neighbors.end(), [](edge& n){
            return n.neighbor.name() == "charlie";
        });
    auto echo_itr = std::find_if(bravo_neighbors.begin(), bravo_neighbors.end(), [](edge& n){
            return n.neighbor.name() == "echo";
        });

    REQUIRE( charlie_itr != bravo_neighbors.end() );
    REQUIRE( echo_itr != bravo_neighbors.end() );

}
