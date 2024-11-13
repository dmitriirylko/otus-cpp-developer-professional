#include <cassert>

#include "matrix.h"

int main()
{
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);
    for(auto c: matrix)
    {
        std::cout << c.first.first << c.first.second << c.second << std::endl;
    }
    return 0;
}