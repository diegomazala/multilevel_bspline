#pragma once


#include <memory>

template <typename scalar_t> class matrix_t
{
  public:

    matrix_t() = delete;

    explicit matrix_t(size_t _rows, size_t _cols)
        : rows(_rows), cols(_cols), data(new scalar_t[_rows * _cols])
    {
    }

    explicit matrix_t(const matrix_t& m)
        : rows(m.rows), cols(m.cols), data(new scalar_t(*m.data))
    {
    }

    explicit matrix_t(const matrix_t &&m) : rows(m.rows), cols(m.cols), data(std::move(m.data))
    {
    }

    matrix_t& operator = (const matrix_t &m)
    {
        this->rows = m.rows;
        this->cols = m.cols;
        this->data = std::make_unique<scalar_t>(*m.data);
        return *this;
    }


  private:
    size_t rows = 0;
    size_t cols = 0;
    std::unique_ptr<scalar_t> data;
};

