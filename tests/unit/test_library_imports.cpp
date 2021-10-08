#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>
#include <utimer.hpp>

TEST_CASE("The timer writes the elapsed time in a variable", "[UTimer]")
{
    long time = -1;
    {
        UTimer t("UTimer creation", &time);
        std::cout.rdbuf(0); // to hide the print of the UTimer destruction
    }
    REQUIRE(time != -1);
}