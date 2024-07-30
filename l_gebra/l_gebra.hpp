/*
 ██╗              ██████╗ ███████╗██████╗ ██████╗  █████╗
 ██║             ██╔════╝ ██╔════╝██╔══██╗██╔══██╗██╔══██╗
 ██║             ██║  ███╗█████╗  ██████╔╝██████╔╝███████║
 ██║             ██║   ██║██╔══╝  ██╔══██╗██╔══██╗██╔══██║
 ███████╗███████╗╚██████╔╝███████╗██████╔╝██║  ██║██║  ██║
 ╚══════╝╚══════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝
 */

/*
 * MIT License
 * Copyright (c) 2024 Rinav
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
--------------------------------| HOW TO USE? |---------------------------------------
| You just need to include this header file in your project and use the vec          |
| class with namespace utl. You also need macro L_GEBRA_IMPLEMENTATION before        |
| "#include "l_gebra"" in one of your source files to include the                    |
| implementation. Basically :- #define L_GEBRA_IMPLEMENTATION #include "l_gebra.hpp" |
--------------------------------------------------------------------------------------
*/

/*
██████╗ ███████╗ ██████╗██╗      █████╗ ██████╗  █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
██╔══██╗██╔════╝██╔════╝██║     ██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
██║  ██║█████╗  ██║     ██║     ███████║██████╔╝███████║   ██║   ██║██║   ██║██╔██╗ ██║
██║  ██║██╔══╝  ██║     ██║     ██╔══██║██╔══██╗██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
██████╔╝███████╗╚██████╗███████╗██║  ██║██║  ██║██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
╚═════╝ ╚══════╝ ╚═════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
*/

#pragma once

#include <uchar.h>

#include <cmath>
#include <cstddef>
#include <cwchar>
#include <initializer_list>
#include <stdexcept>
#include <vector>

#define IL inline
#define S static
#define V virtual

//-------------------------------------------------------------------------------------
//                | NAMESPACE UTL FOR MATRIX AND VECTOR OPERATIONS |
//-------------------------------------------------------------------------------------
namespace utl
{
  // Enum class for solution type
  enum class sol_type
  {
    Unique,
    Infinite,
    NoSolution
  };

  //-------------------------------------------------------------------------------------
  //                               | Matrix class |
  //-------------------------------------------------------------------------------------

  template <typename T>
  class Matrix
  {
  private:
    // Private constructor for the matrix class
    Matrix(size_t rows, size_t cols, std::initializer_list<T> initList) : _rows(rows), _cols(cols), data(initList) {}

  protected:
    size_t _rows;
    size_t _cols;
    std::vector<T> data;

  public:
    //-------------------------------------------------------------------------------------------------
    //                          | CONSTRUCTORS AND DESTRUCTORS |
    //-------------------------------------------------------------------------------------------------

    // Default constructor
    IL Matrix() : _rows(0), _cols(0) {}
    // Constructor to create a matrix of size rows x cols
    IL Matrix(size_t rows, size_t cols) : _rows(rows), _cols(cols), data(rows * cols, 0) {}
    // Constructor to create a matrix of size rows x cols and initialize all elements with val
    IL Matrix(std::initializer_list<std::initializer_list<T>> initList)
        : _rows(initList.size()), _cols(initList.size() > 0 ? initList.begin()->size() : 0)
    {
      size_t size = 0;
      size_t rowSize = _cols;  // Size of the first row
      for (const auto &row : initList)
      {
        size += row.size();
        if (row.size() != rowSize)
          throw std::invalid_argument("Initializer list rows have different sizes");
      }
      data.reserve(size);
      for (const auto &row : initList)
        for (const auto &val : row) data.push_back(val);
    }
    // Constructor to create a row matrix of size rows x 1 and initialize with initList
    IL Matrix(std::initializer_list<T> init_list) : _rows(1), _cols(init_list.size()), data(init_list) {}
    // Move constructor
    // Move constructor
    template <typename U>
    IL Matrix(std::vector<U> &&other) noexcept : _rows(data.size()), _cols(data.empty() ? 0 : data.size() / _rows), data(std::move(other))
    {
    }

    // Constructor to create a matrix of size rows x cols and initialize all elements with val
    Matrix(size_t rows, size_t cols, T val) : _rows(rows), _cols(cols), data(rows * cols, val) {}
    // Constructor to create a column matrix of size rows x 1 and initialize with initList
    Matrix(size_t rows, std::initializer_list<T> initList) : Matrix(rows, 1, initList) {}

    // Copy Constructor
    template <typename Y>
    IL Matrix(const std::vector<Y> &other) : _rows(other.size()), _cols(other.empty() ? 0 : other.size() / _rows)
    {
      for (const auto &row : other)
        for (const auto &val : row) data.push_back(static_cast<T>(val));
    }

    // Destructor
    V IL ~Matrix() = default;

    //-------------------------------------------------------------------------------------------------
    //                   | OPERATOR OVERLOADING & GETTERS/SETTERS |
    //-------------------------------------------------------------------------------------------------

    // Get said paramters of Matrices
    IL size_t size() const { return data.size(); }

    IL size_t rows() const { return _rows; }
    IL size_t rows() { return _rows; }
    IL size_t cols() const { return _cols; }

    IL T &operator()(size_t row, size_t col)
    {
      if (row >= _rows || col >= _cols)
        throw std::out_of_range("Index out of range");
      return data[row * _cols + col];
    }

    IL const T &operator()(size_t row, size_t col) const
    {
      if (row >= rows() || col >= cols())
        throw std::out_of_range("Index out of range");
      return data[row * _cols + col];
    }

    bool operator==(const Matrix<T> &rhs);
    const T &back() { return data.back(); }
    IL void print();

    //--------------------------------| ARITHEMATIC OPERATIONS |--------------------------------------

    // I have used different template for argument and I static cast it to type of object method is being called at
    // You can use (1 / scalar) and  (-1 * scalar) in arguments of * & + to divide and subtract a scalar
    template <typename Y>
    IL Matrix<T> operator+(const Matrix<Y> &other) const;
    template <typename Y>
    IL Matrix<T> operator+(const Y scalar) const;
    template <typename Y>
    IL Matrix<T> operator-(const Matrix<Y> &other) const;
    IL Matrix<T> operator*(const T &scalar) const;
    template <typename Y>
    IL Matrix<T> operator*(const Matrix<Y> &other) const;
    template <typename Y>
    IL Matrix<T> operator/(const Matrix<Y> &other) const;

    //-------------------------------------------------------------------------------------------------
    //                                   |  MATRIX OPERATIONS  |
    //-------------------------------------------------------------------------------------------------

    // Sin and Cos all values in the matrix
    IL void sin();
    IL void cos();

    //---------------------------------------| BASIC UTILITY |------------------------------------------

    // Transpose the matrix
    IL Matrix<T> transpose() const;
    // Calculate the determinant of the matrix
    IL double determinant() const;
    // Calculate co-factor
    IL double cofactor(size_t row, size_t col) const;
    // Get minor by deleting specified row and coloumn
    IL Matrix<T> minor(size_t row, size_t col) const;
    // Get inverse matrix
    IL Matrix<T> inverse();
    // Raise whole matrix to a power
    IL Matrix<T> power(int n);
    // Get rank of matrix
    IL size_t rank() const;
    // Get trace of matrix
    IL T trace() const;
    // Normalize the matrix
    IL double norm() const;

    //-------------------------------| ROW OPERATIONS |--------------------------------------

    // Swap two rows by providing their number
    IL void swap_rows(size_t r1, size_t r2);
    // Multiply row with a scalar
    IL void multiply_row(size_t r, float scalar);
    // Add a scaled row to other, just scale with 1 to simply add
    IL void add_scaled_row(size_t r1, size_t r2, float scalar);

    //----------------------------------| COMPLICATED OPERATIONS |--------------------------------------

    //---------------------------------------------------
    // Eigenvalues and Eigenvectors are best calculated |
    // for symmetric matrices                           |
    // --------------------------------------------------

    //  Get eigenvalues of a matrix using QR algorithm
    IL std::vector<T> eigenvalues();
    // Get dominant eigenvalue using power iteration
    IL std::vector<T> eigenvalues_power_iteration();
    // Get eigenvector using QR algorithm
    IL std::vector<std::vector<T>> eigenvectors();
    // Returns a pair of matrices in form of QR decomposition
    // First one is Q and latter R
    // You can use std::tie(Q, R) = Matrix.qr_decomposition(); where Q & R are matrices
    // or
    // auto QR = X.qr_decomposition();
    // Matrix<T> Q = QR.first;
    // Matrix<T> R = QR.second;
    IL std::pair<Matrix<T>, Matrix<T>> qr_decomposition();
    // Augments two matrices
    IL Matrix<T> augment(const Matrix<T> &other) const;
    // Converts matrix to echelon form
    IL Matrix<T> gauss_elimination() const;
    // Takes a matrix converted to echelon form and returns sol_type to determin if it has solution or not
    // Check sol_type enum class
    S IL sol_type has_solution(const Matrix<T> &reduced_matrix);
    // To be applied on a matrix containing coefficients of variables and
    // to be provided with answers i.e. B, of form AX = B, (B must be coloumn matrix).
    // Returns coloumn matrix of values of variables

    //-------------------------------| EXAMPLE |-------------------------------------------
    // Matrix<float> m1 = {{2.92, 0.86, -1.15}, {0.86, 6.51, 3.32}, {-1.15, 3.32, 4.57}}; |
    // Matrix<float> m2(3, {{3.0}, {1.0}, {-1.0}});                                       |
    // sol_type sol = Matrix<float>::has_solution(m1.gauss_elimination());                |
    // if (sol == sol_type::Unique)                                                       |
    //     cout << "Unique solution" << endl;                                             |
    // else if (sol == sol_type::Infinite)                                                |
    //    cout << "Infinite solutions" << endl;                                           |
    // else                                                                               |
    // cout << "No solution" << endl;                                                     |
    // Matrix<float> ans = m1.solve_linear_system(m2);                                    |
    // OUTPUT:                                                                            |
    //  Unique solution                                                                   |
    //  [1.05438]                                                                         |
    //  [-0.0149266]                                                                      |
    //  [0.0573509]                                                                       |
    //-------------------------------------------------------------------------------------

    IL Matrix<T> solve_linear_system(const Matrix<T> &b) const;

    //------------------------------------------------------------------------------------
    //                         | EXTRA UTILITY FUNCTIONS |
    //------------------------------------------------------------------------------------
    // Get the respective matrices with said dimensions
    S IL Matrix<T> identity_matrix(size_t n);
    S IL Matrix<T> zero_matrix(size_t n);
    S IL Matrix<T> ones_matrix(size_t n);
    S IL Matrix<T> random_matrix(size_t rows, size_t cols, T min, T max);
    // Get min/max of two matrices
    S IL Matrix<T> min(const Matrix<T> &m1, const Matrix<T> &m2);
    S IL Matrix<T> max(const Matrix<T> &m1, const Matrix<T> &m2);
    // Linear interpolation between two matrices
    S IL Matrix<T> lerp(const Matrix<T> &m1, const Matrix<T> &m2, float t);
  };

  //-------------------------------------------------------------------------------------
  //                               | VEC CLASS |
  //-------------------------------------------------------------------------------------

  template <typename T, size_t _size>
  class Vec : public Matrix<T>
  {
  public:
    using Matrix<T>::Matrix;

    //-------------------------------------------------------------------------------------------------
    //                          | CONSTRUCTORS AND DESTRUCTORS |
    //-------------------------------------------------------------------------------------------------

    // Default Constructor
    IL Vec() : Matrix<T>(_size, 1) {}
    // Constructor with initializing list
    IL Vec(std::initializer_list<T> init_list) : Matrix<T>(init_list.size(), init_list) {}
    // Constructor with coloumn matrix
    Vec(const Matrix<T> &matrix) : Matrix<T>(matrix)
    {
      if (matrix.rows() != _size || matrix.cols() != 1)
        throw std::invalid_argument("Invalid matrix dimensions for Vec construction");
    }
    // Copy Constructor
    template <typename Y>
    IL Vec(const std::vector<Y> &other) : Matrix<T>(other)
    {
    }
    // Move Constructor
    template <typename Y>
    IL Vec(const std::vector<Y> &&other) noexcept : Matrix<T>(std::move(other))
    {
    }

    // Copy Constructor for conversion
    template <typename Y>
    IL Vec(const Vec<Y, _size> &other) : Matrix<T>(_size, 1)
    {
      // Copy each element, performing necessary conversions
      for (size_t i = 0; i < _size; ++i) (*this)[i] = static_cast<T>(other[i]);
    }

    // Destructor
    ~Vec() = default;
    //-------------------------------------------------------------------------------------------------
    //                                  | VECTOR FUNCTIONS |
    //-------------------------------------------------------------------------------------------------
    // Print Vector
    IL void print();

    //----------------------------------| GETTERS & SETTERS |------------------------------------------
    IL T x() const
    {
      if (_size >= 1)
        return (*this)[0];
      else
        std::invalid_argument("Can't get x of 0 vector");
    }
    IL T y() const
    {
      if (_size >= 2)
        return (*this)[1];
      else
        std::invalid_argument("Can't get y of vector size <= 1");
    }
    IL T z() const
    {
      if (_size >= 3)
        return (*this)[2];
      else
        std::invalid_argument("Can't get z of vector size <= 2");
    }

    IL size_t size() const { return _size; }
    IL T &operator[](size_t i);
    IL const T &operator[](size_t i) const;

    //--------------------------------------| ARITHEMATIC OPERATIONS |--------------------------------------
    IL Vec operator*(const T x) const;
    template <typename Y, size_t n_x>
    IL Vec operator*(const Vec<Y, n_x> &x) const;
    template <typename Y, size_t n_x>
    IL Vec operator/(const Vec<Y, n_x> &x) const;
    IL Vec operator+(const T x) const;
    template <typename Y, size_t n_x>
    IL Vec operator+(const Vec<Y, n_x> &x) const;
    template <typename Y, size_t n_x>
    IL Vec operator-(const Vec<Y, n_x> &x) const;
    template <typename Y>
    Vec<T, _size> operator*(const Matrix<Y> &m) const;

    //-----------------------------------------------| UTILITY |-------------------------------------------------
    // Calculate elemen wise sin or cos
    IL void sin();
    IL void cos();
    //-------------------------------------------| VECTOR OPERATIONS |-------------------------------------------

    template <typename Y>
    Vec<T, _size> size_aware_multiply(const Matrix<Y> &m) const
    {
      Vec<T, _size> result;

      for (size_t i = 0; i < _size; ++i)
      {
        result[i] = 0;
        for (size_t j = 0; j < m.cols(); ++j)
          if (j < _size)
            result[i] += (*this)[j] * m(j, i);
          else
            result[i] += 1 * m(j, i);
      }
      return result;
    }
    // Dot and cross product
    template <typename Y, size_t n_x>
    IL double dot(const Vec<Y, n_x> &x) const;
    template <typename Y, size_t n_x>
    IL Vec cross(const Vec<Y, n_x> &x) const;
    // Squared and normal magnitude
    IL double squared_magnitude() const;
    IL double magnitude() const;
    // Element wise power
    IL void power(float x);
    // Normalize
    IL float normalize();
    IL Vec<float, _size> get_normalized_vector();
    template <typename Y, size_t n_x>
    // Get angle between two vectors
    IL float angle(const Vec<Y, n_x> &x) const;
    // Get distance between two vectors
    template <typename Y, size_t n_x>
    IL double distance(const Vec<Y, n_x> &x) const;
    // Projection of a vector onto another
    template <typename Y, size_t n_x>
    IL double projection_onto(const Vec<Y, n_x> &x) const;
    // Rotate a vector by an angle
    //-----------------------------| NOTE |---------------------------------
    // This funtion accepts angle in radians and can only                  |
    // rotate 2D and 3D vectors on given Z-axis                            |
    // ** To rotate a 2D vector, you have to use 'z' axis                  |
    // example:                                                            |
    // Vec<double, 3> v3d = {1.0, 0.0, 0.0};                               |
    // v3d.rotate(1.57079632679, 'y');  // Rotate 90 degrees around Y-axis |
    //----------------------------------------------------------------------
    IL Vec<T, _size> rotate(float angle, char8_t axis);
    IL Vec<T, _size> rotate_about_center(const utl::Vec<T, 2> &center, float angle, char8_t axis);
    //------------------------------------------------------------------------------------
    //                         | EXTRA UTILITY FUNCTIONS |
    //------------------------------------------------------------------------------------
    // Get respective vectors with specified paramters
    IL Vec<T, _size> zero_vector();
    IL Vec<T, _size> ones_vector();
    IL Vec<T, _size> random_vector(T min, T max);
    // Get min/ max of Vecs
    S IL Vec<T, _size> min(const Vec<T, _size> &v1, const Vec<T, _size> &v2);
    S IL Vec<T, _size> max(const Vec<T, _size> &v1, const Vec<T, _size> &v2);
    // Linearly interpolate between two vectors
    S IL Vec<T, _size> lerp(const Vec<T, _size> &v1, const Vec<T, _size> &v2, float t);
  };
}  // namespace utl

/*
██████╗ ███████╗███████╗██╗███╗   ██╗██╗████████╗██╗ ██████╗ ███╗   ██╗
██╔══██╗██╔════╝██╔════╝██║████╗  ██║██║╚══██╔══╝██║██╔═══██╗████╗  ██║
██║  ██║█████╗  █████╗  ██║██╔██╗ ██║██║   ██║   ██║██║   ██║██╔██╗ ██║
██║  ██║██╔══╝  ██╔══╝  ██║██║╚██╗██║██║   ██║   ██║██║   ██║██║╚██╗██║
██████╔╝███████╗██║     ██║██║ ╚████║██║   ██║   ██║╚██████╔╝██║ ╚████║
╚═════╝ ╚══════╝╚═╝     ╚═╝╚═╝  ╚═══╝╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
*/

#ifdef L_GEBRA_IMPLEMENTATION

#include <iomanip>
#include <iostream>
#include <limits>
#include <ostream>
#include <tuple>

namespace utl
{
  template <typename T>
  bool Matrix<T>::operator==(const Matrix<T> &rhs)
  {
    if ((*this).rows() != rhs.rows() || (*this).cols() != rhs.cols())
      return false;

    for (size_t i = 0; i < (*this).rows(); ++i)
    {
      for (size_t j = 0; j < (*this).cols(); ++j)
        if ((*this)(i, j) != rhs(i, j))
          return false;
    }

    return true;
  }
  template <typename T>
  void Matrix<T>::print()
  {
    // Find the maximum width for each column
    std::vector<size_t> col_widths(_cols, 0);
    for (size_t j = 0; j < _cols; ++j)
    {
      for (size_t i = 0; i < _rows; ++i)
      {
        size_t width = std::to_string((*this)(i, j)).size();
        if (width > col_widths[j])
          col_widths[j] = width;
      }
    }

    // Print the matrix
    for (size_t i = 0; i < _rows; ++i)
    {
      std::cout << "[ ";
      for (size_t j = 0; j < _cols; ++j) std::cout << std::setw(col_widths[j]) << (*this)(i, j) << " ";
      std::cout << "]\n";
    }
  }

  template <typename T>
  template <typename Y>
  IL Matrix<T> Matrix<T>::operator+(const Matrix<Y> &other) const
  {
    if (_rows != other.rows() || _cols != other.cols())
      throw std::invalid_argument("Matrix dimensions must match for addition");

    Matrix<T> result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i)
      for (size_t j = 0; j < _cols; ++j) result(i, j) = (*this)(i, j) + static_cast<T>(other(i, j));
    return result;
  }

  template <typename T>
  template <typename Y>
  IL Matrix<T> Matrix<T>::operator+(const Y scalar) const
  {
    Matrix<T> result = *this;
    for (size_t i = 0; i < _rows; ++i)
      for (size_t j = 0; j < _cols; ++j) result(i, j) = result(i, j) + static_cast<T>(scalar);
    return result;
  }

  template <typename T>
  template <typename Y>
  IL Matrix<T> Matrix<T>::operator-(const Matrix<Y> &other) const
  {
    if (_rows != other.rows() || _cols != other.cols())
      throw std::invalid_argument("Matrix dimensions must match for subtraction");

    Matrix<T> result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i)
      for (size_t j = 0; j < _cols; ++j) result(i, j) = (*this)(i, j) - static_cast<T>(other(i, j));
    return result;
  }

  template <typename T>
  IL Matrix<T> Matrix<T>::operator*(const T &scalar) const
  {
    Matrix<T> result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i)
      for (size_t j = 0; j < _cols; ++j) result(i, j) = (*this)(i, j) * scalar;
    return result;
  }

  template <typename T>
  template <typename Y>
  IL Matrix<T> Matrix<T>::operator*(const Matrix<Y> &other) const
  {
    if (_cols != other.rows())
      throw std::invalid_argument("Matrix dimensions do not match for multiplication");

    Matrix<T> result(_rows, other.cols());
    for (size_t i = 0; i < _rows; ++i)
    {
      for (size_t j = 0; j < other.cols(); ++j)
      {
        T sum = static_cast<T>(0);
        for (size_t k = 0; k < _cols; ++k) sum += (*this)(i, k) * static_cast<T>(other(k, j));
        result(i, j) = sum;
      }
    }
    return result;
  }

  template <typename T>
  template <typename Y>
  IL Matrix<T> Matrix<T>::operator/(const Matrix<Y> &other) const
  {
    if (_rows != other.rows() || _cols != other.cols())
      throw std::invalid_argument("Matrix dimensions must match for element-wise division");

    Matrix<T> result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i)
    {
      for (size_t j = 0; j < _cols; ++j)
      {
        if (other(i, j) == 0)
          throw std::runtime_error("Division by zero");
        result(i, j) = (*this)(i, j) / (T)other(i, j);
      }
    }
    return result;
  }

  template <typename T>
  IL void Matrix<T>::sin()
  {
    for (auto &val : data) val = std::sin(val);
  }

  template <typename T>
  IL void Matrix<T>::cos()
  {
    for (auto &val : data) val = std::cos(val);
  }

  template <typename T>
  IL Matrix<T> Matrix<T>::transpose() const
  {
    Matrix<T> result(_cols, _rows);
    for (size_t i = 0; i < _rows; ++i)
      for (size_t j = 0; j < _cols; ++j) result(j, i) = (*this)(i, j);
    return result;
  }

  template <typename T>
  double Matrix<T>::determinant() const
  {
    if (_rows != _cols)
      throw std::invalid_argument("Matrix must be square to compute determinant");

    if (_rows == 1)
    {
      return data[0];
    }
    else if (_rows == 2)
    {
      return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
    }
    else
    {
      double det = 0;
      int sign = 1;
      for (size_t i = 0; i < _cols; ++i)
      {
        det += sign * (*this)(0, i) * minor(0, i).determinant();
        sign = -sign;
      }
      return det;
    }
  }

  template <typename T>
  Matrix<T> Matrix<T>::minor(size_t row, size_t col) const
  {
    if (_rows <= 1 || _cols <= 1)
      throw std::invalid_argument("Matrix must have dimensions greater than 1 to compute minor");

    Matrix<T> result(_rows - 1, _cols - 1);
    size_t m = 0, n = 0;
    for (size_t i = 0; i < _rows; ++i)
    {
      if (i == row)
        continue;
      n = 0;
      for (size_t j = 0; j < _cols; ++j)
      {
        if (j == col)
          continue;
        result(m, n++) = (*this)(i, j);
      }
      if (i != row)
        ++m;
    }
    return result;
  }

  template <typename T>
  double Matrix<T>::cofactor(size_t row, size_t col) const
  {
    double factor = minor(row, col).determinant();
    return ((row + col) % 2 == 0) ? factor : -factor;
  }

  template <typename T>
  Matrix<T> Matrix<T>::inverse()
  {
    Matrix<T> m = *this;

    if (m.rows() != m.cols())
      throw std::invalid_argument("Matrix must be square to compute inverse");

    const double det = m.determinant();
    if (std::abs(det) < std::numeric_limits<double>::epsilon())
      throw std::runtime_error("Matrix is singular and has no inverse");

    // Check for singularity before creating the result matrix
    if (m.rows() == 1)
    {
      // 1x1 matrix
      return Matrix<T>({1.0 / m(0, 0)});
    }
    else if (m.rows() == 2)
    {
      // 2x2 matrix
      const T a = m(0, 0), b = m(0, 1), c = m(1, 0), d = m(1, 1);
      const T inv_det = static_cast<T>(1.0 / det);
      return Matrix<T>({{d * inv_det, -b * inv_det}, {-c * inv_det, a * inv_det}});
    }

    Matrix<T> result(m.rows(), m.cols());
    for (size_t i = 0; i < m.rows(); ++i)
      for (size_t j = 0; j < m.cols(); ++j) result(i, j) = m.cofactor(i, j) / det;

    return result.transpose();
  }

  template <typename T>
  Matrix<T> Matrix<T>::power(int n)
  {
    if (_rows != _cols)
      throw std::invalid_argument("Matrix must be square to compute power");

    if (n < 0)
      return inverse(*this).power(-n);

    Matrix<T> result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) result(i, i) = 1;

    Matrix<T> x = *this;
    while (n)
    {
      if (n & 1)
        result = result * x;
      x = x * x;
      n >>= 1;
    }
    return result;
  }

  template <typename T>
  size_t Matrix<T>::rank() const
  {
    Matrix<T> reduced = this->gauss_elimination().first;
    size_t rank = 0;
    for (size_t i = 0; i < _rows; ++i)
    {
      bool is_zero_row = true;
      for (size_t j = 0; j < _cols; ++j)
      {
        if (reduced(i, j) != 0)
        {
          is_zero_row = false;
          break;
        }
      }
      if (!is_zero_row)
        ++rank;
    }
    return rank;
  }
  template <typename T>
  T Matrix<T>::trace() const
  {
    if (_rows != _cols)
      throw std::invalid_argument("Matrix must be square to compute trace");

    T result = 0;
    for (size_t i = 0; i < _rows; ++i) result += (*this)(i, i);
    return result;
  }
  template <typename T>
  double Matrix<T>::norm() const
  {
    T result = 0;
    for (size_t i = 0; i < _rows; ++i)
      for (size_t j = 0; j < _cols; ++j) result += (*this)(i, j) * (*this)(i, j);
    return std::sqrt(result);
  }
  template <typename T>
  void Matrix<T>::swap_rows(size_t r1, size_t r2)
  {
    if (r1 >= _rows || r2 >= _rows)
      throw std::out_of_range("Row index out of range");

    for (size_t j = 0; j < _cols; ++j) std::swap((*this)(r1, j), (*this)(r2, j));
  }

  // Multiply a row by a scalar value
  template <typename T>
  void Matrix<T>::multiply_row(size_t r, float scalar)
  {
    if (r >= _rows)
      throw std::out_of_range("Row index out of range");

    for (size_t j = 0; j < _cols; ++j) (*this)(r, j) *= scalar;
  }

  // Add a scalar multiple of one row to another row
  template <typename T>
  void Matrix<T>::add_scaled_row(size_t r1, size_t r2, float scalar)
  {
    if (r1 >= _rows || r2 >= _rows)
      throw std::out_of_range("Row index out of range");

    for (size_t j = 0; j < _cols; ++j) (*this)(r1, j) += scalar * (*this)(r2, j);
  }

  template <typename T>
  std::vector<T> Matrix<T>::eigenvalues()
  {
    Matrix<float> A = *this;
    int max_iter = 1000;
    float epsilon = 1e-5;
    int iter = 0;
    Matrix<T> pq = Matrix<T>::identity_matrix(_rows);
    for (iter = 0; iter < max_iter; ++iter)
    {
      Matrix<float> Q(A.rows(), A.cols(), 0);
      Matrix<float> R(A.rows(), A.cols(), 0);
      std::tie(Q, R) = A.qr_decomposition();

      A = R * Q;
      pq = pq * Q;

      // Check for convergence
      bool converged = true;
      for (size_t i = 1; i < A.rows(); ++i)
      {
        if (std::abs(A(i - 1, i)) > epsilon)
        {
          std::cout << "\n"
                    << "Q Matrix: \n";
          Q.print();
          std::cout << '\n' << "R Matrix: \n";
          R.print();

          converged = false;
          break;
        }
      }
      if (converged)
        break;
    }

    std::vector<float> eigenvalues;
    for (size_t i = 0; i < A.rows(); ++i) eigenvalues.push_back(A(i, i));

    return eigenvalues;
  }
  // Function to perform power iteration method
  template <typename T>
  std::vector<T> Matrix<T>::eigenvalues_power_iteration()
  {
    Matrix<T> m = *this;
    const size_t n = m.rows();
    std::vector<T> eigenvalues;

    // Maximum number of iterations
    size_t max_iter = 1000;
    // Convergence criterion
    T epsilon = 1e-5;

    for (size_t i = 0; i < n; ++i)
    {
      // Start with a vector of all 1s
      Matrix<T> x(n, 1, 1.0);

      for (size_t iter = 0; iter < max_iter; ++iter)
      {
        // Perform matrix-vector multiplication
        Matrix<T> y = m * x;

        // Obtain the eigenvalue
        T lambda = y(i, 0);

        // Normalize the eigenvector
        y = y * (1 / lambda);

        // Check for convergence
        if ((y - x).norm() < epsilon)
        {
          eigenvalues.push_back(lambda);
          break;
        }

        // Update the eigenvector for the next iteration
        x = y;
      }

      // Deflate the matrix
    }

    return eigenvalues;
  }

  template <typename T>
  std::vector<std::vector<T>> Matrix<T>::eigenvectors()
  {
    Matrix<T> X = *this;
    Matrix<T> pQ = Matrix<T>::identity_matrix(_rows);

    // Iterate until convergence
    const int maxIterations = 100;
    for (int i = 0; i < maxIterations; ++i)
    {
      auto QR = X.qr_decomposition();
      Matrix<T> Q = QR.first;
      Matrix<T> R = QR.second;

      X = R * Q;
      pQ = pQ * Q;
    }

    // Normalize eigenvectors
    std::vector<std::vector<T>> eigenvectors;
    for (size_t i = 0; i < _cols; ++i)
    {
      std::vector<T> eigenvector(_rows);
      for (size_t j = 0; j < _rows; ++j) eigenvector[j] = pQ(j, i);

      // Normalize eigenvector
      /*  T norm = std::sqrt(std::inner_product(eigenvector.begin(), eigenvector.end(), eigenvector.begin(),
              0.0)); for (size_t j = 0; j < _rows; ++j)
              {
                  eigenvector[j] /= norm;
              }
  */
      eigenvectors.push_back(eigenvector);
    }

    return eigenvectors;
  }

  template <typename T>
  std::pair<Matrix<T>, Matrix<T>> Matrix<T>::qr_decomposition()
  {
    if (_rows != _cols)
      throw std::invalid_argument("QR decomposition requires a square matrix");

    Matrix<T> Q(*this);  // Start with Q as a copy of the input matrix
    Matrix<T> R(_rows, _cols, 0);

    for (size_t k = 0; k < _cols; ++k)
    {
      // Compute the kth column of Q
      for (size_t i = 0; i < k; ++i)
      {
        T dotProduct = 0;
        for (size_t j = 0; j < _rows; ++j) dotProduct += Q(j, k) * Q(j, i);
        for (size_t j = 0; j < _rows; ++j) Q(j, k) -= dotProduct * Q(j, i);
      }

      // Compute the norm and normalize the kth column of Q
      T norm = 0;
      for (size_t j = 0; j < _rows; ++j) norm += Q(j, k) * Q(j, k);
      norm = std::sqrt(norm);

      if (norm < 1e-10)
        throw std::runtime_error("Matrix is singular or nearly singular");

      for (size_t j = 0; j < _rows; ++j) Q(j, k) /= norm;

      // Compute the kth row of R
      for (size_t i = k; i < _cols; ++i)
      {
        T dotProduct = 0;
        for (size_t j = 0; j < _rows; ++j) dotProduct += Q(j, k) * (*this)(j, i);
        R(k, i) = dotProduct;
      }
    }

    return std::make_pair(Q, R);
  }

  template <typename T>
  Matrix<T> Matrix<T>::augment(const Matrix<T> &other) const
  {
    if (_rows != other._rows)
      throw std::invalid_argument("Matrix dimensions must match for augmentation");

    Matrix<T> augmented(_rows, _cols + other._cols);
    for (size_t i = 0; i < _rows; ++i)
    {
      for (size_t j = 0; j < _cols; ++j) augmented(i, j) = (*this)(i, j);
      for (size_t j = 0; j < other._cols; ++j) augmented(i, _cols + j) = other(i, j);
    }
    return augmented;
  }

  template <typename T>
  Matrix<T> Matrix<T>::gauss_elimination() const
  {
    Matrix<float> result = (*this);

    int lead = 0;
    int n_rows = result.rows();

    while (lead < n_rows)
    {
      float divisor, multiplier;
      for (size_t r = 0; r < n_rows; ++r)
      {
        divisor = 1 / (float)(result(lead, lead));

        multiplier = (float)result(r, lead) / (float)result(lead, lead) * (-1);
        if (r == lead)
          result.multiply_row(r, divisor);
        else
          result.add_scaled_row(r, lead, multiplier);
      }
      lead++;
    }
    return result;
  }

  template <typename T>
  IL sol_type Matrix<T>::has_solution(const Matrix<T> &reduced_matrix)
  {
    size_t non_zero_rows = 0;
    for (size_t i = 0; i < reduced_matrix.rows(); ++i)
    {
      bool is_zero_row = true;
      for (size_t j = 0; j < reduced_matrix.cols(); ++j)
      {
        if (reduced_matrix(i, j) != 0)
        {
          is_zero_row = false;
          break;
        }
      }
      if (!is_zero_row)
        ++non_zero_rows;
    }

    if (non_zero_rows == reduced_matrix.cols())
      return sol_type::Unique;
    else if (non_zero_rows < reduced_matrix.cols())
      return sol_type::Infinite;
    else
      return sol_type::NoSolution;
  }

  template <typename T>
  Matrix<T> Matrix<T>::solve_linear_system(const Matrix<T> &b) const
  {
    if (_rows != _cols || _rows != b.rows())
      throw std::invalid_argument("Invalid matrix and vector dimensions");

    Matrix<T> augmented = augment(b);
    Matrix<T> reduced_matrix = augmented.gauss_elimination();
    sol_type solution_type = has_solution(*this);

    if (solution_type == sol_type::NoSolution)
      throw std::runtime_error("No solution exists for the linear system");

    if (solution_type == sol_type::Infinite)
      throw std::runtime_error("Infinite solutions exist for the linear system");

    Matrix<T> x(_cols, 1);
    if (solution_type == sol_type::Unique)
    {
      for (int i = _rows - 1; i >= 0; --i)
      {
        T sum = reduced_matrix(i, _cols);
        for (size_t j = i + 1; j < _cols; ++j) sum -= reduced_matrix(i, j) * x(j, 0);

        x(i, 0) = sum / reduced_matrix(i, i);
      }
    }
    return x;
  }

  template <typename T>
  IL Matrix<T> Matrix<T>::identity_matrix(size_t n)
  {
    Matrix<T> result(n, n);
    for (size_t i = 0; i < n; ++i) result(i, i) = static_cast<T>(1);
    return result;
  }

  template <typename T>
  IL Matrix<T> Matrix<T>::zero_matrix(size_t n)
  {
    Matrix<T> result(n, n);
    for (size_t i = 0; i < n; ++i)
      for (size_t j = 0; j < n; ++j) result(i, j) = static_cast<T>(0);
    return result;
  }

  template <typename T>
  IL Matrix<T> Matrix<T>::ones_matrix(size_t n)
  {
    Matrix<T> result(n, n);
    for (size_t i = 0; i < n; ++i)
      for (size_t j = 0; j < n; ++j) result(i, j) = static_cast<T>(1);
    return result;
  }

  template <typename T>
  IL Matrix<T> Matrix<T>::random_matrix(size_t rows, size_t cols, T min, T max)
  {
    Matrix<T> result(rows, cols);
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = 0; j < cols; ++j) result(i, j) = min + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (max - min)));
    return result;
  }
  template <typename T>
  IL Matrix<T> Matrix<T>::min(const Matrix<T> &m1, const Matrix<T> &m2)
  {
    if (m1.rows() != m2.rows() || m1.cols() != m2.cols())
      throw std::invalid_argument("Matrix dimensions must match for element-wise minimum");
    Matrix<T> result(m1.rows(), m1.cols());
    if (m1.magnitude() > m2.magnitude())
      result = m2;
    else
      result = m1;
    return result;
  }

  template <typename T>
  IL Matrix<T> Matrix<T>::max(const Matrix<T> &m1, const Matrix<T> &m2)
  {
    if (m1.rows() != m2.rows() || m1.cols() != m2.cols())
      throw std::invalid_argument("Matrix dimensions must match for comparison");
    Matrix<T> result(m1.rows(), m1.cols());
    if (m1.magnitude() > m2.magnitude())
      result = m1;
    else
      result = m2;
    return result;
  }

  template <typename T>
  IL Matrix<T> Matrix<T>::lerp(const Matrix<T> &m1, const Matrix<T> &m2, float t)
  {
    if (m1.rows() != m2.rows() || m1.cols() != m2.cols())
      throw std::invalid_argument("Matrix dimensions must match for linear interpolation");
    Matrix<T> result(m1.rows(), m1.cols());

    for (size_t i = 0; i < m1.rows(); ++i)
      for (size_t j = 0; j < m1.cols(); ++j) result(i, j) = m1(i, j) * (1 - t) + m2(i, j) * t;
    return result;
  }

  // Definitions of Vec member functions
  template <typename T, size_t _size>
  IL void Vec<T, _size>::print()
  {
    std::cout << "[ ";
    for (size_t i = 0; i < _size; ++i) std::cout << (*this)[i] << " ";
    std::cout << "]\n";
  }

  template <typename T, size_t _size>
  IL T &Vec<T, _size>::operator[](size_t i)
  {
    if (i >= _size)
      throw std::out_of_range("Index out of range");
    return this->operator()(i, 0);
  }

  template <typename T, size_t _size>
  IL const T &Vec<T, _size>::operator[](size_t i) const
  {
    if (i >= _size)
      throw std::out_of_range("Index out of range");
    return this->operator()(i, 0);
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::operator*(const T x) const
  {
    Vec<T, _size> result;
    for (size_t i = 0; i < _size; ++i) result[i] = (*this)[i] * x;
    return result;
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL Vec<T, _size> Vec<T, _size>::operator*(const Vec<Y, n_x> &x) const
  {
    if (_size != n_x)
      throw std::invalid_argument("Vector sizes do not match for multiplication");
    Vec<T, _size> result;
    for (size_t i = 0; i < _size; ++i) result[i] = (*this)[i] * x[i];
    return result;
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL Vec<T, _size> Vec<T, _size>::operator/(const Vec<Y, n_x> &x) const
  {
    if (_size != n_x)
      throw std::invalid_argument("Vector sizes do not match for division");
    Vec<T, _size> result;
    for (size_t i = 0; i < _size; ++i)
    {
      if (x[i] == 0)
        throw std::invalid_argument("Division by zero");
      result[i] = (*this)[i] / x[i];
    }
    return result;
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::operator+(const T x) const
  {
    Vec<T, _size> result;
    for (size_t i = 0; i < _size; ++i) result[i] = (*this)[i] + x;
    return result;
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL Vec<T, _size> Vec<T, _size>::operator+(const Vec<Y, n_x> &x) const
  {
    if (_size != n_x)
      throw std::invalid_argument("Vector sizes do not match for addition");
    Vec<T, _size> result;
    for (size_t i = 0; i < _size; ++i) result[i] = (*this)[i] + x[i];
    return result;
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL Vec<T, _size> Vec<T, _size>::operator-(const Vec<Y, n_x> &x) const
  {
    if (_size != n_x)
      throw std::invalid_argument("Vector sizes do not match for subtraction");
    Vec<T, _size> result;
    for (size_t i = 0; i < _size; ++i) result[i] = (*this)[i] - x[i];
    return result;
  }

  template <typename T, size_t _size>
  template <typename Y>
  Vec<T, _size> Vec<T, _size>::operator*(const Matrix<Y> &m) const
  {
    if (m.cols() != _size)
      throw std::invalid_argument("Vector and matrix dimensions do not match for multiplication");

    Vec<T, _size> result;
    for (size_t i = 0; i < m.rows(); ++i)
    {
      T sum = static_cast<T>(0);
      for (size_t j = 0; j < _size; ++j) sum += (*this)[j] * m(i, j);
      result[i] = sum;
    }
    return result;
  }

  template <typename T, size_t _size>
  IL void Vec<T, _size>::sin()
  {
    for (size_t i = 0; i < _size; ++i) (*this)[i] = std::sin((*this)[i]);
  }

  template <typename T, size_t _size>
  IL void Vec<T, _size>::cos()
  {
    for (size_t i = 0; i < _size; ++i) (*this)[i] = std::cos((*this)[i]);
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL double Vec<T, _size>::dot(const Vec<Y, n_x> &x) const
  {
    if (_size != n_x)
      throw std::invalid_argument("Vector sizes do not match for dot product");
    double result = 0;
    for (size_t i = 0; i < _size; ++i) result += (*this)[i] * x[i];
    return result;
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL Vec<T, _size> Vec<T, _size>::cross(const Vec<Y, n_x> &x) const
  {
    if (_size != 3 || n_x != 3)
      throw std::invalid_argument("Cross product is defined only for 3-dimensional vectors");
    Vec<T, _size> result;
    result[0] = (*this)[1] * x[2] - (*this)[2] * x[1];
    result[1] = (*this)[2] * x[0] - (*this)[0] * x[2];
    result[2] = (*this)[0] * x[1] - (*this)[1] * x[0];
    return result;
  }

  template <typename T, size_t _size>
  IL double Vec<T, _size>::squared_magnitude() const
  {
    double sum = 0;
    for (size_t i = 0; i < _size; ++i) sum += (*this)[i] * (*this)[i];
    return sum;
  }

  template <typename T, size_t _size>
  IL double Vec<T, _size>::magnitude() const
  {
    double sum = 0;
    for (size_t i = 0; i < _size; ++i) sum += (*this)[i] * (*this)[i];
    return sqrt(sum);
  }

  template <typename T, size_t _size>
  IL void Vec<T, _size>::power(float x)
  {
    if (!std::isfinite(x))
      throw std::invalid_argument("Power must be a finite number");

    for (size_t i = 0; i < _size; ++i) (*this)[i] = std::pow((*this)[i], x);
  }

  template <typename T, size_t _size>
  IL float Vec<T, _size>::normalize()
  {
    double mag = magnitude();
    if (mag == 0)
      throw std::runtime_error("Cannot normalize a zero vector");
    for (size_t i = 0; i < _size; ++i) (*this)[i] /= mag;
    return static_cast<float>(mag);
  }

  template <typename T, size_t _size>
  IL Vec<float, _size> Vec<T, _size>::get_normalized_vector()
  {
    Vec<float, _size> result;
    double mag = magnitude();
    if (mag == 0)
      throw std::runtime_error("Cannot normalize a zero vector");
    for (size_t i = 0; i < _size; ++i) result[i] = static_cast<float>((*this)[i] / mag);
    return result;
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL float Vec<T, _size>::angle(const Vec<Y, n_x> &x) const
  {
    double dot_product = dot(x);
    double this_magnitude = magnitude();
    double x_magnitude = x.magnitude();

    if (this_magnitude == 0 || x_magnitude == 0)
      throw std::runtime_error("Cannot compute angle for zero vector");

    double cos_theta = dot_product / (this_magnitude * x_magnitude);
    return acos(static_cast<float>(cos_theta));
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL double Vec<T, _size>::distance(const Vec<Y, n_x> &x) const
  {
    if (_size != n_x)
      throw std::invalid_argument("Vector sizes do not match for distance calculation");
    double sum = 0;
    for (size_t i = 0; i < _size; ++i) sum += ((*this)[i] - x[i]) * ((*this)[i] - x[i]);
    return sqrt(sum);
  }

  template <typename T, size_t _size>
  template <typename Y, size_t n_x>
  IL double Vec<T, _size>::projection_onto(const Vec<Y, n_x> &x) const
  {
    if (x.magnitude() == 0)
      throw std::runtime_error("Cannot project onto a zero vector");
    return dot(x) / (x.magnitude() * x.magnitude());
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::rotate(float angle, char8_t axis)
  {
    if (_size == 2)
    {
      if (axis != 'z' && axis != 'Z')
        throw std::invalid_argument("For vectors of size 2, only rotation around the Z-axis is supported");

      Vec<T, _size> result;

      double cosA = std::cos(angle);
      double sinA = std::sin(angle);

      double x = (*this)[0];
      double y = (*this)[1];
      result[0] = x * cosA - y * sinA;
      result[1] = x * sinA + y * cosA;

      *this = result;
    }
    else if (_size == 3)
    {
      Vec<T, _size> result;

      double cosA = std::cos(angle);
      double sinA = std::sin(angle);

      switch (axis)
      {
        case 'x':
        case 'X':
          result[0] = (*this)[0];
          result[1] = (*this)[1] * cosA - (*this)[2] * sinA;
          result[2] = (*this)[1] * sinA + (*this)[2] * cosA;
          break;

        case 'y':
        case 'Y':
          result[0] = (*this)[0] * cosA + (*this)[2] * sinA;
          result[1] = (*this)[1];
          result[2] = -(*this)[0] * sinA + (*this)[2] * cosA;
          break;

        case 'z':
        case 'Z':
          result[0] = (*this)[0] * cosA - (*this)[1] * sinA;
          result[1] = (*this)[0] * sinA + (*this)[1] * cosA;
          result[2] = (*this)[2];
          break;

        default:
          throw std::invalid_argument("Invalid rotation axis");
      }
      return result;
    }
    else
    {
      throw std::invalid_argument("Rotation only supported for vectors of size 2 or 3");
    }
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::rotate_about_center(const utl::Vec<T, 2> &center, float angle, char8_t axis)
  {
    utl::Vec<T, _size> point = *this;

    if (_size == 2)
    {
      if (axis != 'z' && axis != 'Z')
        throw std::invalid_argument("For vectors of size 2, only rotation around the Z-axis is supported");

      double s = std::sin(angle);
      double c = std::cos(angle);

      // Translate point back to origin
      double x = (*this).x() - center.x();
      double y = (*this).y() - center.y();

      // Rotate point
      double x_new = x * c - y * s;
      double y_new = x * s + y * c;

      // Translate point back
      x = x_new + center.x();
      y = y_new + center.y();

      return {static_cast<T>(x), static_cast<T>(y)};
    }
    else if (_size == 3)
    {
      Vec<T, _size> result;
      double cosA = std::cos(angle);
      double sinA = std::sin(angle);

      // Translate point to center
      double x = (*this)[0] - center.x();
      double y = (*this)[1] - center.y();
      double z = (*this)[2] - center.z();

      switch (axis)
      {
        case 'x':
        case 'X':
          result[0] = x;
          result[1] = y * cosA - z * sinA;
          result[2] = y * sinA + z * cosA;
          break;
        case 'y':
        case 'Y':
          result[0] = x * cosA + z * sinA;
          result[1] = y;
          result[2] = -x * sinA + z * cosA;
          break;
        case 'z':
        case 'Z':
          result[0] = x * cosA - y * sinA;
          result[1] = x * sinA + y * cosA;
          result[2] = z;
          break;
        default:
          throw std::invalid_argument("Invalid rotation axis");
      }

      // Translate back
      result[0] += center.x();
      result[1] += center.y();
      result[2] += center.z();

      return result;
    }
    else
    {
      throw std::invalid_argument("Rotation only supported for vectors of size 2 or 3");
    }
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::zero_vector()
  {
    for (size_t i = 0; i < _size; ++i) (*this)[i] = 0;
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::ones_vector()
  {
    for (size_t i = 0; i < _size; ++i) (*this)[i] = 1;
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::random_vector(T min, T max)
  {
    for (size_t i = 0; i < _size; ++i) (*this)[i] = min + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (max - min)));
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::min(const Vec<T, _size> &v1, const Vec<T, _size> &v2)
  {
    if (v1.size() != v2.size())
      throw std::invalid_argument("Vector sizes do not match for element-wise minimum");
    Vec<T, _size> result;
    if (v1.magnitude() < v2.magnitude())
      result = v1;
    else
      result = v2;
    return result;
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::max(const Vec<T, _size> &v1, const Vec<T, _size> &v2)
  {
    if (v1.size() != v2.size())
      throw std::invalid_argument("Vector sizes do not match for element-wise minimum");
    Vec<T, _size> result;
    if (v1.magnitude() > v2.magnitude())
      result = v1;
    else
      result = v2;
    return result;
  }

  template <typename T, size_t _size>
  IL Vec<T, _size> Vec<T, _size>::lerp(const Vec<T, _size> &v1, const Vec<T, _size> &v2, float t)
  {
    Vec<T, _size> result;
    for (size_t i = 0; i < _size; ++i) result[i] = v1[i] * (1 - t) + v2[i] * t;
    return result;
  }

}  // namespace utl

#endif  // L_GEBRA_IMPLEMENTATION
