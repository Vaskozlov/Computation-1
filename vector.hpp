#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <valarray>

class Vector : public std::valarray<double>
{
public:
    using std::valarray<double>::valarray;
};

#endif /* VECTOR_HPP */
