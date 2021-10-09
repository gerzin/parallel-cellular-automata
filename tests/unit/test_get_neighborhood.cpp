/*
 * I test the private member functions of the seq::CellularAutomaton by copypasting
 * here the code of the function and defining the right global variables.
 * It's ugly but still less ugly than something like #define private public, #define protected public
 * or modifying the original code by inserting in it a friend class which potentially could alter
 * the behaviour of the code.
 */
#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>
#include <cellular_automata.hpp>

const size_t rows = 4;
const size_t columns = 5;

/*
 * 0 -2 0 0 -1
 * 0 1 2 3 -2
 * 0 4 0 5 0
 * 9 6 7 8 0
 */

const int grid[rows][columns] = {{0, -2, 0, 0, -1}, {0, 1, 2, 3, -2}, {0, 4, 0, 5, 0}, {9, 6, 7, 8, 0}};

template <typename T>
std::tuple<T, T, T, T, T, T, T, T> get_neighborhood(int row, int col)
{
    T top_left, top, top_right, left, right, bottom_left, bottom, bottom_right;
    top_left = grid[(row - 1 + rows) % rows][(col - 1 + columns) % columns];
    top = grid[(row - 1 + rows) % rows][col];
    top_right = grid[(row - 1 + rows) % rows][(col + 1) % columns];
    left = grid[row][(col - 1 + columns) % columns];
    right = grid[row][(col + 1) % columns];
    bottom_left = grid[(row + 1) % rows][(col - 1 + columns) % columns];
    bottom = grid[(row + 1) % rows][col];
    bottom_right = grid[(row + 1) % rows][(col + 1) % columns];
    return std::make_tuple(top_left, top, top_right, left, right, bottom_left, bottom, bottom_right);
};

TEST_CASE("The right neighborhood is computed.", "[get_neigborhood]")
{
    auto [tl, t, tr, l, r, bl, b, br] = get_neighborhood<int>(2, 2);
    REQUIRE(tl == 1);
    REQUIRE(t == 2);
    REQUIRE(tr == 3);
    REQUIRE(l == 4);
    REQUIRE(r == 5);
    REQUIRE(bl == 6);
    REQUIRE(b == 7);
    REQUIRE(br == 8);

    SECTION("The neighborhood is computed on a toroidal surface.")
    {
        auto [tl, t, tr, l, r, bl, b, br] = get_neighborhood<int>(0, 0);
        REQUIRE(tl == 8);
        REQUIRE(t == 0);
        REQUIRE(tr == 9);
        REQUIRE(l == -1);
        REQUIRE(r == 0);
        REQUIRE(bl == -2);
        REQUIRE(b == 0);
        REQUIRE(br == 1);
    }
}