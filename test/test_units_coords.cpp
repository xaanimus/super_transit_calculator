#include <iostream>
#include <catch.hpp>
#include <units_coords.hpp>

using namespace stransit;

TEST_CASE("distance to") {
    geo_coords a {0,0};
    geo_coords b {0,0.02};

    CHECK(a.distance_to(b) == Approx(1.381).epsilon(0.01) );
}

TEST_CASE("hello") {
    geo_coords a {0,0};
    geo_coords b {0.02, 0};

    std::cout << "dist: " << a.distance_to(b) << std::endl;
    std::cout << delta_geo_angle(1.38195) << std::endl;
}

