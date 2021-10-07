#define CONFIG_CATCH_MAIN
#include <catch/catch.hpp>
#include <utimer.hpp>

TEST_CASE("The timer writes the elapsed time in a variable", "UTimer")
{
    long time = 0;
    {
        UTimer t("UTimer creation", &time);
    }
    REQUIRE(time != 0);
}