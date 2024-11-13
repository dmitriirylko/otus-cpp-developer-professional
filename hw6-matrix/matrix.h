#pragma once

#include <iostream>
#include <map>
#include <utility>

template<typename T, T defaultValue> class MatrixProxy;

template<typename T, T defaultValue>
class Matrix
{
public:
    Matrix() = default;

    ~Matrix() = default;

    size_t size()
    {
        return m_data.size();
    }

    void print()
    {
        for(const auto &x : m_data)
        {
            std::cout << "[" << x.first.first
                      << ", " << x.first.second << "] = "
                      << x.second << std::endl;
        }
    }

    MatrixProxy<T, defaultValue> operator[](size_t row)
    {
        return MatrixProxy<T, defaultValue>(*this, row);
    }

    using iterator = typename std::map<std::pair<size_t, size_t>, T>::iterator;
    using const_iterator = typename std::map<std::pair<size_t, size_t>, T>::const_iterator;

    iterator begin()
    {
        return m_data.begin();
    }

    iterator end()
    {
        return m_data.end();
    }

    const_iterator cbegin()
    {
        return m_data.cbegin();
    }

    const_iterator cend()
    {
        return m_data.cend();
    }

private:
    std::map<std::pair<size_t, size_t>, T> m_data;
    friend MatrixProxy<T, defaultValue>;
};

template<typename T, T defaultValue>
class MatrixProxy
{
public:
    MatrixProxy(Matrix<T, defaultValue> &matrix, size_t row) :
        m_matrix(matrix),
        m_row(row)
    {}

    MatrixProxy& operator[](size_t col)
    {
        m_col = col;
        return *this;
    } 

    void operator=(T value)
    {
        auto idx = std::make_pair(m_row, m_col);
        if(value == defaultValue)
        {
            auto iter = m_matrix.m_data.find(idx);
            if(iter == m_matrix.m_data.end())
            {
                return;
            }
            m_matrix.m_data.erase(iter);
            return;
        }
        m_matrix.m_data[idx] = value;
    }

    operator T()
    {
        auto idx = std::make_pair(m_row, m_col);
        if(m_matrix.m_data.find(idx) == m_matrix.m_data.end())
        {
            return defaultValue;
        }
        return m_matrix.m_data[idx];
    }

private:
    size_t m_row;
    size_t m_col;
    Matrix<T, defaultValue> &m_matrix;
};
