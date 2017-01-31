#include <iostream>
#include <catch.hpp>
#include <quadtree.hpp>
#include <unordered_set>

using namespace stransit;

TEST_CASE("quadtree with nodes") {
    quadtree<node*, node::get_node_position> tree;
    stop_node nodes[] = {
        stop_node("charlie", 1, day_set_monday, "Outbound", {12h, 30min},
                  edge_storage(), {0min, nullptr}, miles(10), {0, 3.2}),
        stop_node("delta", 1, day_set_monday, "Outbound", {12h, 30min},
                  edge_storage(), {0min, nullptr}, miles(10), {2.32, 2.3}),
        stop_node("echo", 1, day_set_monday, "Outbound", {12h, 30min},
                  edge_storage(), {0min, nullptr}, miles(10), {0.121, 0.1}),
        stop_node("bravo", 1, day_set_monday, "Outbound", {12h, 30min},
                  edge_storage(), {0min, nullptr}, miles(10), {1, 0}),
        stop_node("alpha", 1, day_set_monday, "Outbound", {12h, 30min},
                  edge_storage(), {0min, nullptr}, miles(10), {0, 0}),
    };

    for (node& n: nodes) {
        tree.insert(&n);
    }

    std::cout << "printing:" << std::endl;
    std::vector<node*> ns = tree.search_range({0.0, 0.0, 2.0, 2.0});
    std::unordered_set<std::string> names;
    for (node* n : ns) {
        names.insert(n->name());
    }

    std::string required_names[] = {"echo", "bravo", "alpha"};

    for (std::string& required_name : required_names) {
        auto itr = names.find(required_name);
        CHECK( itr != names.end() );
    }
}
