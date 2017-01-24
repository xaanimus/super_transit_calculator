#include <iostream>
#include <algorithm>
#include <catch.hpp>
#include "node_database.hpp"

using namespace std::literals;

node_database::node_storage::iterator find_node(node_database::node_storage& storage,
                                                std::string name) {
    return std::find_if(storage.begin(), storage.end(), [&name](node* n){
            return n->name() == name;
        });
}

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

    node_database db(blank_transit_info, 1min, 0, {}, {}, DAY_ANY, {12h, 0min});
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

    node_database db(info, 50min, 0, {}, {}, DAY_ANY, {12h, 0min});

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

TEST_CASE("connect nodes walking") {
    
    stop_info_schedule sched_1 = {
        .route_number = 1,
        .day = "Monday",
        .direction = "Outbound",
        .stops = {
            {"alpha", 1, "Monday", "Outbound", time_hm(2h, 30min), {0, 0}},
            {"bravo", 1, "Monday", "Outbound", time_hm(2h, 34min), {0, 0.01}},
            {"charlie", 1, "Monday", "Outbound", time_hm(2h, 40min), {0, 0.02}},
        }
    };

    stop_info_schedule sched_2 = {
        .route_number = 2,
        .day = "Monday",
        .direction = "Outbound",
        .stops = {
            {"delta", 1, "Monday", "Outbound", time_hm(2h, 30min), {0.001, 0}},
            {"echo", 1, "Monday", "Outbound", time_hm(2h, 34min), {0.001, 0.01}},
            {"foxtrot", 1, "Monday", "Outbound", time_hm(2h, 40min), {0.001, 0.02}},
        }
    };

    transit_info info = {{sched_1, sched_2}};

    node_database db(info, 50min, miles(0.1), {}, {}, DAY_ANY, {12h, 0min});

    node_database::node_storage& nodes = db.nodes();

    auto alpha_node = std::find_if(nodes.begin(), nodes.end(), [](node* n)->bool{
            return n->name() == "alpha";
        });

    REQUIRE( alpha_node != nodes.end());

    std::vector<edge>& neighbors = (*alpha_node)->neighbors();
    int count = 0;
    for (edge& e: neighbors) {
        if (e.neighbor.name() == "bravo" ||
            e.neighbor.name() == "delta") {
            count++;
        }
    }

    CHECK( count == 2);
}

TEST_CASE ("heuristic") {
    
    stop_info_schedule sched_1 = {
        .route_number = 1,
        .day = "Monday",
        .direction = "Outbound",
        .stops = {
            {"alpha", 1, "Monday", "Outbound", time_hm(2h, 30min), {1.2, 0.3}},
            {"bravo", 1, "Monday", "Outbound", time_hm(2h, 34min), {0, 0.01}},
            {"charlie", 1, "Monday", "Outbound", time_hm(2h, 40min), {0, 0.02}},
        }
    };

    geo_coords end_points[] = {
        { 1     , 0.04},
        { 2     , 0.056},
        { 34    , 0.1},
        { 1     , 0.24},
        { 0.1   , 2.34},
        { 0.432 , 42.04},
    };

    for (geo_coords& end_point: end_points) {
        node_database db({{sched_1}}, 0min, 0, {0,0}, end_point, DAY_ANY, {12h, 0min});
        std::chrono::minutes expected_heuristic =
            walking_time(geo_coords(1.2, 0.3).distance_to(end_point));

        node_database::node_storage nodes = db.nodes();
        auto alpha_itr = find_node(nodes, "alpha");
        CHECK( alpha_itr != nodes.end() );
        std::chrono::minutes h = db.heuristic(**alpha_itr);
        INFO( "end point:" << end_point.longitude << "," << end_point.latitude );
        CHECK( h == expected_heuristic );
    }
}
