#include <catch.hpp>
#include <units_coords.hpp>

using namespace stransit;

TEST_CASE("distance to") {
    geo_coords a {0,0};
    geo_coords b {0,0.02};

    CHECK(a.distance_to(b) == Approx(1.381).epsilon(0.01) );
}
