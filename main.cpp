#include "columns_reorder.hpp"
#include "matrix.hpp"
#include "vector.hpp"

#include <cassert>
#include <expected>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <valarray>
#include <vector>

class SystemSolver
{
private:
    Matrix c;
    Vector d;
    std::size_t reorderingCount{};
    double matrixNorm{};

    SystemSolver(Matrix c, Vector d, const std::size_t reorderingCount, const double matrix_norm)
      : c(std::move(c))
      , d(std::move(d))
      , reorderingCount(reorderingCount)
      , matrixNorm(matrix_norm)
    {}

public:
    struct SolutionResult
    {
        Vector result;
        std::size_t iterations;
        Vector error;
        double norm;
    };

    static std::expected<SystemSolver, std::string_view>
        buildSolver(const Matrix &input_matrix, const Vector &input_vector)
    {
        auto reordered_matrix = input_matrix;
        auto reordered_vector = input_vector;

        const auto reordering_result =
            equations::tryToReorderColumns(reordered_matrix, reordered_vector);

        if (!reordering_result.has_value()) {
            return std::unexpected(
                "Failed to reorder columns in such way, that main element is dominant");
        }

        for (std::size_t y = 0; y != reordered_matrix.getRowsCount(); ++y) {
            const auto inversed_divider = 1.0 / reordered_matrix[y, y];

            for (std::size_t x = 0; x != reordered_matrix.getColsCount(); ++x) {
                reordered_matrix[y, x] = x == y ? 0.0 : -reordered_matrix[y, x] * inversed_divider;
            }

            reordered_vector[y] *= inversed_divider;
        }

        const auto norm = reordered_matrix.computeNorm();

        if (norm > 1.0) {
            return std::unexpected("||C|| > 1 – can not solve");
        }

        return SystemSolver(
            std::move(reordered_matrix),
            std::move(reordered_vector),
            reordering_result.value(),
            norm);
    }

    auto solve(const double precision) -> std::expected<SolutionResult, std::string_view>
    {
        assert(precision > 0 && "Precision can not be negative");

        SolutionResult solution_result{d, 0, Vector(d.size()), matrixNorm};
        auto &[solution, iterations, error, norm] = solution_result;

        double max_distance = precision * 2;

        while (max_distance > precision) {
            max_distance = 0.0;

            for (std::size_t row = 0; row != c.getRowsCount(); ++row) {
                auto x = d[row];

                for (std::size_t col = 0; col < c.getColsCount(); ++col) {
                    x += c[row, col] * solution[col];
                }

                error[row] = std::abs(x - solution[row]);
                max_distance = std::max(max_distance, error[row]);
                solution[row] = x;
            }

            ++iterations;
        }

        return solution_result;
    }
};

int main(const int argc, const char *argv[])
{
    std::unique_ptr<std::ifstream> input_stream{};

    if (argc == 2) {
        const auto *input_file = argv[1];
        input_stream = std::make_unique<std::ifstream>(input_file);
        std::cin.rdbuf(input_stream->rdbuf());
    }

    double precision = 0.001;
    intmax_t n_input = 0;

    std::cin >> n_input;

    if (n_input < 0) {
        fmt::println(stderr, "Matrix size can not be negative");
    }

    std::cin >> precision;

    if (precision < 0.0) {
        fmt::println(stderr, "Precision can not be negative");
    }

    const auto n = static_cast<std::size_t>(n_input);
    auto matrix = Matrix(n, n);
    auto vector = Vector(n);

    for (std::size_t y = 0; y != n; ++y) {
        for (std::size_t x = 0; x != n; ++x) {
            std::cin >> matrix[y, x];
        }

        std::cin >> vector[y];
    }

    auto buildResult = SystemSolver::buildSolver(matrix, vector);

    if (!buildResult.has_value()) {
        fmt::println(stderr, "Failed to build solver: {}", buildResult.error());
    }

    auto &solver = buildResult.value();
    const auto solution = solver.solve(precision);

    if (solution.has_value()) {
        auto [r, i, e, n] = solution.value();
        fmt::println("Iterations: {}\nError vector: {}\nNorm: {}\nSolution vector: {}", i, e, n, r);
    } else {
        fmt::print("{}", solution.error());
    }

    return 0;
}
