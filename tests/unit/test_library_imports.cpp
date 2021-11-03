#include "../../include/utimer.hpp"
// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on

TEST_CASE("The timer writes the elapsed time in a variable", "[UTimer]")
{
    long time = -1;
    {
        UTimer t("UTimer creation", &time);
        std::cout.rdbuf(0); // to hide the print of the UTimer destruction
    }
    REQUIRE(time != -1);
}