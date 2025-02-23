#ifndef COLUMNS_REORDER_HPP
#define COLUMNS_REORDER_HPP

#include "matrix.hpp"
#include "vector.hpp"

#include <expected>
#include <string_view>

namespace equations
{
    std::expected<std::size_t, std::string_view>
        tryToReorderColumns(Matrix &matrix, Vector &vector);
} // namespace equations

#endif /* COLUMNS_REORDER_HPP */
