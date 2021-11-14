/*
 * I test the private member functions of the seq::CellularAutomaton by copypasting
 * here the code of the function and defining the right global variables.
 * It's ugly but still less ugly than something like #define private public, #define protected public
 * or modifying the original code by inserting in it a friend class which potentially could alter
 * the behaviour of the code.
 */
#include "../../include/cellular_automata.hpp"
// clang-format off
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "../ext/catch/catch.hpp"
// clang-format on
#include <memory>

/*
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
*/

TEST_CASE("The neighborhood is computed correctly.")
{
    const size_t rows = 4;
    const size_t columns = 5;

    /* Grid structure
     *
     * 0 -2  0  0  -1
     * 0  1  2  3  -2
     * 0  4  0  5   0
     * 9  6  7  8   0
     */

    const int grid[rows][columns] = {{0, -2, 0, 0, -1}, {0, 1, 2, 3, -2}, {0, 4, 0, 5, 0}, {9, 6, 7, 8, 0}};

    // the code in this lambda is an exact copy of the code from the get_neighborhood
    // function in the CellularAutomaton class, where T has been instantiated with an int.
    auto get_neighborhood = [&grid](int row, int col) -> auto
    {
        int top_left, top, top_right, left, right, bottom_left, bottom, bottom_right;
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

    SECTION("The function returns the neighbors in the correct order")
    {
        auto [tl, t, tr, l, r, bl, b, br] = get_neighborhood(2, 2);
        REQUIRE(tl == 1);
        REQUIRE(t == 2);
        REQUIRE(tr == 3);
        REQUIRE(l == 4);
        REQUIRE(r == 5);
        REQUIRE(bl == 6);
        REQUIRE(b == 7);
        REQUIRE(br == 8);
    }

    SECTION("The neighborhood is computed on a toroidal surface.")
    {
        auto [tl, t, tr, l, r, bl, b, br] = get_neighborhood(0, 0);
        REQUIRE(tl == 0);
        REQUIRE(t == 9);
        REQUIRE(tr == 6);
        REQUIRE(l == -1);
        REQUIRE(r == -2);
        REQUIRE(bl == -2);
        REQUIRE(b == 0);
        REQUIRE(br == 1);
    }
}

TEST_CASE("The sequential cellular automaton is created.")
{
    const auto rows = 5;
    const auto columns = 5;
    auto grid = std::make_unique<int *[]>(rows);

    for (auto i = 0; i < rows; ++i)
    {
        grid[i] = new int[columns];
    }
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
        {
            grid[i][j] = i + j;
        }
    }

    auto update_fn = [](int c, int tl, int t, int tr, int l, int r, int bl, int b, int br) -> int {
        return c + tl + t + tr + l + r + bl + b + br + 1;
    };
    auto automaton = ca::seq::CellularAutomaton<int>(grid.get(), rows, columns, update_fn);
    std::cout << automaton << std::endl;

    automaton.simulate(0);
    automaton.simulate(1);
    automaton.simulate(0);
    automaton.simulate(2);

    REQUIRE(automaton.get_generation() == 3);
}