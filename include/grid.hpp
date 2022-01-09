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
 * @tparam T type of the cell.
 */
template <typename T>
class Grid
{
  public:
    /**
     * @brief Construct a new Grid object.
     *
     * @param rows number of rows of the grid.
     * @param cols number of columns of the grid.
     * @throws invalid_argument if either rows or cols are zero.
     */
    Grid(size_t rows, size_t cols) : nrows(rows), ncols(cols)
    {
        if (!(nrows & ncols))
        {
            throw std::invalid_argument("Grid constructor has 0 size.");
        }
        grid.reserve(nrows * ncols);
    }
    // TODO: add move and copy constructor.

    /**
     * @b.rief Return a grid of the same dimension of the grid passed as argument
     *
     * @param other  Grid whose dimension has to be taken.
     * @return Grid new grid of same dimension.
     */
    static Grid newWithSameSize(const Grid &other)
    {
        return Grid(other.nrows, other.ncols);
    }
    /**
     * @brief Get the element in position row,col.
     *
     * @param row row number.
     * @param col column number.
     * @pre 0 <= rows < this->nrows
     * @pre 0 <= col < this->ncols
     * @return T& reference to the element in position row,col
     */
    T &operator()(unsigned row, unsigned col)
    {
        return grid[ncols * row + col];
    }
    /**
     * @brief Get the element in position row,col.
     *
     * @param row row number.
     * @param col column number.
     * @pre 0 <= rows < this->nrows
     * @pre 0 <= col < this->ncols
     * @return T& reference to the element in position row,col
     */
    T operator()(unsigned row, unsigned col) const
    {
        return grid[ncols * row + col];
    }

    /**
     * @brief Returns the number of rows of the grid.
     *
     * @return size_t number of rows.
     */
    inline size_t rows() const
    {
        return this->nrows;
    }
    /**
     * @brief Returns the number of columns of the grid.
     *
     * @return size_t  number of columns.
     */
    inline size_t columns() const
    {
        return this->columns;
    }
    /**
     * @brief Prints the grid on the stream, with the elements in a row separated by a whitespace and the rows separated
     * by a newline.
     *
     * @param os output stream on which to print.
     * @param grid grid to print.
     * @return std::ostream&
     */
    friend std::ostream &operator<<(std::ostream &os, const Grid &grid)
    {

        for (size_t i = 0; i < grid.nrows; ++i)
        {
            for (size_t j = 0; j < grid.ncols; ++j)
            {
                os << grid(i, j) << " ";
            }

            os << std::endl;
        }

        return os;
    }

    void swap(Grid &other)
    {
        using std::swap;
        std::cout << "Custom Swap Used" << std::endl;
        swap(nrows, other.nrows);
        swap(ncols, other.ncols);
        grid.swap(other.grid);
    }

  private:
    std::vector<T> grid;
    size_t nrows;
    size_t ncols;
};

} // namespace ca