// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
#include<grid.hpp>

TEST_CASE("The grid works correctly") {
    SECTION("Grid Creation") {
        auto g = ca::Grid<int>(10,10);
        REQUIRE(g.columns() == 10);
        REQUIRE(g.rows() == 10);

        REQUIRE(g.getInnerVector().size() == 100);
    }

    SECTION("New grid with same size and equality") {
        auto g = ca::Grid<int>(10,10);
        auto h = ca::Grid<int>::newWithSameSize(g);
        REQUIRE(g == h);
    }

    SECTION("Setting the first row") {
        auto g = ca::Grid<int>(10,10);
        for(unsigned i = 0; i < 10; ++i){
            g(0,i) = 1;
        }

        REQUIRE(g(0,2) == 1);

        auto h = ca::Grid<int>::newWithSameSize(g);

        REQUIRE_FALSE(h == g);
    }
}