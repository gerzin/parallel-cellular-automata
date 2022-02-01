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
#ifndef PARALLEL_CELLULAR_AUTOMATA_GRID_HPP
#define PARALLEL_CELLULAR_AUTOMATA_GRID_HPP

#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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
    Grid(size_t rows, size_t cols) : grid(rows * cols), nrows(rows), ncols(cols)
    {
        if (!grid.size())
        {
            throw std::invalid_argument("Grid constructor has 0 size.");
        }
    }

    /**
     * @brief Construct a new Grid object.
     *
     * @param other grid to copy.
     */
    Grid(const Grid &other)
    {
        nrows = other.nrows;
        ncols = other.ncols;
        grid = other.grid;
    }

    /**
     * @brief Construct a new Grid object.
     *
     * @param other grid to copy.
     */
    Grid(Grid &&other)
    {
        nrows = other.nrows;
        ncols = other.ncols;
        grid = std::move(other.grid);
    }

    /**
     * @brief Return a grid of the same dimension of the grid passed as argument.
     *
     * @param other  Grid whose dimension has to be taken.
     * @return Grid new grid of same dimension.
     */
    static Grid newWithSameSize(const Grid &other)
    {
        return Grid(other.nrows, other.ncols);
    }

    /**
     * @brief Load a grid from a file.
     *
     * The file must contain the number of rows, the number of columns and then the elements of the grid.
     *
     * @param filepath path to the file.
     * @return Grid grid initialized with the content of the file.
     */

    static Grid newFromFile(std::string filepath, bool is_binary = false)
    {
        std::ifstream f(filepath);
        if (f)
        {

            unsigned rows, columns;
            f >> rows;
            f >> columns;
            std::vector<T> v;
            v.reserve(rows * columns);
            T value;
            while (f >> value)
            {
                v.push_back(value);
            }
            for (auto e : v)
            {
                std::cout << e << " ";
            }

            return Grid(std::move(v), rows, columns);
        }
        else
        {
            throw std::invalid_argument("Invalid filepath");
        }
    }
    /**
     * @brief Writes the grid to a file.
     *
     * @param filepath path of the file.
     */
    void toFile(std::string filepath)
    {
        std::ofstream file(filepath);
        if (file)
        {
            file << nrows << " " << ncols << std::endl;
            for (const auto elem : grid)
            {
                file << elem << " ";
            }
        }
        else
        {
            throw std::invalid_argument("Invalid filepath");
        }
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
        return this->ncols;
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
    /**
     * @brief Compare two grids for equality.
     *
     * @return true if the grids are equal.
     * @return false if the grids differ.
     */
    friend bool operator==(const Grid &lhs, const Grid &rhs)
    {
        return (lhs.nrows == rhs.nrows) && (lhs.ncols == rhs.ncols) &&
               std::equal(lhs.grid.begin(), lhs.grid.end(), rhs.grid.begin());
    }

    /**
     * @brief Swap the content of the grid with the one of another grid.
     *
     * @param other grid to swap.
     */
    void swap(Grid &other)
    {
        using std::swap;
        swap(nrows, other.nrows);
        swap(ncols, other.ncols);
        grid.swap(other.grid);
    }

    /**
     * @brief Get a reference to the vector representing the grid.
     *
     * @return std::vector<T>& vector representing the grid.
     */
    std::vector<T> &getInnerVector()
    {
        return grid;
    }

  private:
    Grid(std::vector<T> &&v, size_t rows, size_t cols) : grid(std::move(v)), nrows(rows), ncols(cols)
    {
        if (!(getInnerVector().size() && rows && cols))
        {
            throw std::invalid_argument("Empty vector or invalid dimension");
        }
    }
    /**
     * @brief The grid is represented as a 1D vector.
     *
     */
    std::vector<T> grid;
    /**
     * @brief Number of rows of the grid.
     *
     */
    size_t nrows;
    /**
     * @brief Number of columns of the grid.
     *
     */
    size_t ncols;
};

} // namespace ca
#endif
