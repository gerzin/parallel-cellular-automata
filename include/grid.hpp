/**
 * @file grid.hpp
 * @author Gerardo Zinno (g.zinno@studenti.unipi.it)
 * @brief Definition of the grid of the automaton.
 * @version 0.1
 * @date 2022-01-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <array>
#include <memory>
namespace ca
{

/**
 * @brief Grid of the cellular automaton.
 *
 * @tparam T
 */
template <typename T>
class Grid
{
  public:
    Grid(size_t rows, size_t cols) : nrows(rows), ncols(cols)
    {
        grid.reserve(nrows * ncols);
    }

    /**
     * @brief Returns a pointer to the beginning of the indexed row.
     *
     * @param index index of the row.
     * @return const T*
     */
    const T *operator[](int index) const
    {
        return &grid[index];
    }

    static Grid newWithSameSize(const Grid &other)
    {
        return Grid(other.nrows, other.ncols);
    }

    size_t rows() const
    {
        return this->nrows;
    }

    size_t columns() const
    {
        return this->columns;
    }

    friend std::ostream &operator<<(std::ostream &os, const Grid &grid)
    {

        for (size_t i = 0; i < grid.nrows; ++i)
        {
            for (size_t j = 0; j < grid.ncols; ++j)
            {
                os << grid[i][j] << " ";
            }

            os << std::endl;
        }

        return os;
    }

    friend void swap(Grid &a, Grid &b)
    {
        using std::swap;
        swap(a.nrwow, b.nrows);
        swap(a.ncols, b.ncols);
        a.grid.swap(b.grid);
    }

    std::vector<T> &getGridVector()
    {
        return this->grid;
    }

  private:
    std::vector<T> grid;
    size_t nrows;
    size_t ncols;
};

} // namespace ca