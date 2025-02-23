#include "columns_reorder.hpp"

#include <vector>

namespace equations
{
    std::expected<std::size_t, std::string_view>
        getDominantValueIndex(const std::size_t row, const Matrix &matrix)
    {
        std::size_t result = 0;
        auto max_value = std::abs(matrix[row, 0]);
        auto elements_sum = max_value;

        for (std::size_t i = 1; i != matrix.getRowsCount(); ++i) {
            const auto cur_value = std::abs(matrix[row, i]);

            if (cur_value > max_value) {
                max_value = cur_value;
                result = i;
            }

            elements_sum += cur_value;
        }

        if (max_value < elements_sum - max_value) {
            return std::unexpected("failed to reorder columns");
        }

        return result;
    }

    std::expected<std::size_t, std::string_view> tryToReorderColumns(Matrix &matrix, Vector &vector)
    {
        std::size_t reordering_count = 0;
        std::vector new_indices(matrix.getRowsCount(), std::numeric_limits<std::size_t>::max());

        for (std::size_t row = 0; row != matrix.getRowsCount(); ++row) {
            const auto dominant_value_index = getDominantValueIndex(row, matrix);

            if (!dominant_value_index.has_value()) {
                return dominant_value_index;
            }

            const auto index = dominant_value_index.value();

            if (new_indices[index] != std::numeric_limits<std::size_t>::max()) {
                return std::unexpected("failed to reorder columns");
            }

            reordering_count = static_cast<std::size_t>(row != index);
            new_indices[index] = row;
        }

        Matrix new_matrix(matrix.getRowsCount(), matrix.getColsCount());
        Vector new_vector(matrix.getColsCount());

        for (std::size_t row = 0; row != matrix.getRowsCount(); ++row) {
            const auto source_row = new_indices[row];

            for (std::size_t col = 0; col != matrix.getColsCount(); ++col) {
                new_matrix[row, col] = matrix[source_row, col];
            }

            new_vector[row] = vector[source_row];
        }

        matrix = new_matrix;
        vector = new_vector;

        return reordering_count;
    }
} // namespace equations
