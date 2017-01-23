/*
  Tests for time_hm
 */

#include <catch.hpp>
#include <transit_time.hpp>

#include <chrono>
using namespace std::literals;

struct time_test_item {
    stransit::time_hm first;
    stransit::time_hm second;
    std::chrono::minutes expected;
};

TEST_CASE("time_hm::difference_to") {

    time_test_item test_items[] = {
        {stransit::time_hm(1h, 0min), stransit::time_hm(2h, 0min), 1h},
        {stransit::time_hm(10h, 0min), stransit::time_hm(20h, 0min), 10h},
        {stransit::time_hm(19h, 30min), stransit::time_hm(22h, 0min), 2h + 30min},
        {stransit::time_hm(19h, 30min), stransit::time_hm(3h, 12min), 7h + 42min},
    };

    for (auto test : test_items) {
        std::chrono::minutes diff = test.first.difference_to(test.second);
        INFO( test.first.to_string() << " and " << test.second.to_string() );
        INFO( "Difference is " << diff.count() << "min");
        INFO( "Difference should be " << test.expected.count() << "min");
        CHECK( diff == test.expected );
    }

}
