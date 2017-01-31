//#define TEST_SEARCH_DISABLED

#ifndef TEST_SEARCH_DISABLED

#include <iostream>
#include <fstream>
#include <catch.hpp>
#include <super_transit.hpp>

#include <chrono>
using namespace std::literals;
using namespace stransit;
using namespace stransit::literals;

TEST_CASE("search1") {
    stop_info_schedule sched1 = {
        .route_number = 1,
        .day = day_set_monday,
        .direction = "Outbound",
        .stops = {
            {"alpha", 1, day_set_monday, "Outbound", {12h, 30min}, {1.22, 3.2}},
            {"bravo", 1, day_set_monday, "Outbound", {12h, 40min}, {1.22, 3.202}},
            {"charlie", 1, day_set_monday, "Outbound", {13h, 03min}, {1.2201, 3.21}},
            {"delta", 1, day_set_monday, "Outbound", {13h, 30min}, {1.3204, 3.22}},
        }
    };

    stop_info_schedule sched2 = {
        .route_number = 2,
        .day = day_set_monday,
        .direction = "Outbound",
        .stops = {
            {"echo", 2, day_set_monday, "Outbound", {12h, 31min}, {1.22001, 3.2001}},
            {"foxtrot", 2, day_set_monday, "Outbound", {12h, 50min}, {1.223, 3.202}},
            {"charlie", 2, day_set_monday, "Outbound", {13h, 05min}, {1.2201, 3.21}},
            {"golf", 2, day_set_monday, "Outbound", {13h, 15min}, {1.3205, 3.2202}},
        }
    };

    geo_coords start_loc = {1.2205, 3.2};
    geo_coords end_loc = {1.32045, 3.22};

    trip_options options = {
        .start_position = start_loc,
        .start_time = {12h, 0min},
        .start_day = day_type::day_monday,
        .info = {
            .stop_schedules = {sched1, sched2}
        },
        .end_position = end_loc,
        .max_wait_time = 1h,
        .max_walk_dist = 1.0_mi,
    };

    /*
    std::vector<trip> result = get_trips(options);
    */
}

TEST_CASE("parse json") {
    std::ifstream locations_stream("stop-locations.json");
    std::ifstream schedules_stream("stop-schedules.json");

    std::string loc_str((std::istreambuf_iterator<char>(locations_stream)),
                        std::istreambuf_iterator<char>());
    std::string sched_str((std::istreambuf_iterator<char>(schedules_stream)),
                          std::istreambuf_iterator<char>());

    try {
        transit_info info = generate_transit_info_from_json(sched_str, loc_str);
        REQUIRE( info.stop_schedules.size() > 0);
    } catch(std::string& s) {
        INFO (s);
        REQUIRE(false);
    } catch(const char * s) {
        INFO (s);
        REQUIRE(false);
    } catch(...) {
        REQUIRE(false);
    }
}

TEST_CASE("find airport -> statler") {

    std::ifstream locations_stream("/Users/serge-olivieramega/Developer/bravo/super_transit_calculator/stop-locations.json");
    std::ifstream schedules_stream("/Users/serge-olivieramega/Developer/bravo/super_transit_calculator/stop-schedules.json");

    std::string loc_str((std::istreambuf_iterator<char>(locations_stream)),
                        std::istreambuf_iterator<char>());
    std::string sched_str((std::istreambuf_iterator<char>(schedules_stream)),
                          std::istreambuf_iterator<char>());

    transit_info info;
    try {
        info = generate_transit_info_from_json(sched_str, loc_str);
        REQUIRE( info.stop_schedules.size() > 0);
    } catch(std::string& s) {
        INFO (s);
        REQUIRE(false);
    } catch(const char * s) {
        INFO (s);
        REQUIRE(false);
    } catch(...) {
        REQUIRE(false);
    }

    trip_options options = {
        .start_position = {-76.4626, 42.489067},
        .start_time = {10h, 0min},
        .start_day = day_type::day_monday, //doesn't make sense, will fix this later
        .info = info,
        .end_position = {-76.482567, 42.445983},
        .max_wait_time = 14min,
        .max_walk_dist = 0.5_mi
    };

    std::cout << "hi" << std::endl;
    std::vector<trip> trips = get_trips(options);
    std::cout << trips.size() << std::endl;
}

#endif // TEST_SEARCH_DISABLED
