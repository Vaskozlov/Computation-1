#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cmath>
#include <cstddef>
#include <valarray>

class Matrix
{
    std::valarray<double> data;
    std::size_t rows;
    std::size_t cols;

public:
    Matrix(const std::size_t rows, const std::size_t cols)
      : data(rows * cols)
      , rows(rows)
      , cols(cols)
    {}

    auto operator[](const std::size_t row, const std::size_t col) -> double &
    {
        return data[row * cols + col];
    }

    [[nodiscard]] auto operator[](const std::size_t row, const std::size_t col) const -> double
    {
        return data[row * cols + col];
    }

    auto begin() -> decltype(std::begin(data))
    {
        return std::begin(data);
    }

    auto end() -> decltype(std::end(data))
    {
        return std::end(data);
    }

    [[nodiscard]] auto size() const -> std::size_t
    {
        return data.size();
    }

    [[nodiscard]] auto getRowsCount() const -> std::size_t
    {
        return rows;
    }

    [[nodiscard]] auto getColsCount() const -> std::size_t
    {
        return cols;
    }

    [[nodiscard]] auto computeNorm() const -> double
    {
        double result = 0.0;

        for (size_t i = 0; i < rows * cols; ++i) {
            result += data[i] * data[i];
        }

        return std::sqrt(result);
    }
};

#endif /* MATRIX_HPP */
