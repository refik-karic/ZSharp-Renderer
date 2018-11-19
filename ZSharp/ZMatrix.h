#ifndef ZMATRIX_H
#define ZMATRIX_H

#include <cstddef>
#include <cmath>

#include "ZVector.h"

namespace ZSharp {

template <std::size_t rows, std::size_t cols , typename T>
class ZMatrix {
  public:
  enum class Axis {
    TWO_DIMENSIONS,
    X,
    Y,
    Z
  };

  ZMatrix() {

  }

  ZMatrix(const ZMatrix<rows, cols, T>& copy) {
    // Self copy guard.
    if (this == &copy) {
      return;
    }

    *this = copy;
  }

  void operator=(const ZMatrix<rows, cols, T>& matrix) {
    // Self assignment guard.
    if (this == &matrix) {
      return;
    }

    for (std::size_t row = 0; row < rows; row++) {
      mData[row] = matrix[row];
    }
  }

  ZVector<cols, T>& operator[](std::size_t index) {
    return mData[index];
  }

  ZMatrix<rows, cols, T> operator*(T scalar) {
    ZMatrix<rows, cols, T> result;

    for (std::size_t row = 0; row < rows; row++) {
      result[row] = mData[row] * scalar;
    }

    return result;
  }

  ZMatrix<rows, cols, T> operator*(const ZMatrix<rows, cols, T>& matrix) {
    ZMatrix<rows, cols, T> result;

    // Transpose the right hand side to allow a simple dot product on each row.
    ZMatrix<rows, cols, T> rhsTranspose(ZMatrix<rows, cols, T>::Transpose(matrix));

    for (std::size_t row = 0; row < rows; row++) {
      for (std::size_t col = 0; col < cols; col++) {
        result[row][col] = mData[row] * rhsTranspose[col];
      }
    }

    return result;
  }

  static void Identity(ZMatrix<rows, cols, T>& matrix) {
    T zero{};
    T one{1};

    for (std::size_t row = 0; row < rows; row++) {
      for (std::size_t col = 0; col < cols; col++) {
        if (row == col) {
          matrix[row][col] = one;
        }
        else {
          matrix[row][col] = zero;
        }
      }
    }
  }
  
  static void Clear(ZMatrix<rows, cols, T>& matrix) {
    for (std::size_t row = 0; row < rows; row++) {
      ZVector<cols, T>::Clear(matrix[row]);
    }
  }

  static ZMatrix<rows, cols, T> Transpose(const ZMatrix<rows, cols, T>& matrix) {
    ZMatrix<rows, cols, T> result;

    for (std::size_t row = 0; row < rows; row++) {
      for (std::size_t col = 0; col < cols; col++) {
        result[row][col] = matrix[col][row];
      }
    }

    return result;
  }

  static void SetTranslation(ZMatrix<rows, cols, T>& matrix, const ZVector<cols, T>& translation) {
    std::size_t lastColumn = cols - 1;
    for (std::size_t row = 0; row < rows; row++) {
      matrix[row][lastColumn] = translation[row];
    }
  }

  static void SetScale(ZMatrix<rows, cols, T>& matrix, const ZVector<cols, T>& scale) {
    for (std::size_t row = 0; row < rows; row++) {
      matrix[row][row] = scale[row];
    }
  }

  static void SetRotation(ZMatrix<rows, cols, T>& matrix, T angle, Axis axis) {
    // TODO: Find a way to reduce type conversion here and call the appropriate type-specific trig functions, perhaps using type-traits.
    switch (axis) {
      case Axis::Z:
      case Axis::TWO_DIMENSIONS:
        matrix[0][0] = cos(reinterpret_cast<double>(angle));
        matrix[0][1] = -1 * sin(reinterpret_cast<double>(angle));
        matrix[1][0] = sin(reinterpret_cast<double>(angle));
        matrix[1][1] = cos(reinterpret_cast<double>(angle));
        matrix[2][2] = 1;

        if (axis == Axis::Z) {
          matrix[3][3] = 1;
        }
        break;
      case Axis::X:
        matrix[0][0] = 1;
        matrix[1][1] = cos(reinterpret_cast<double>(angle));
        matrix[1][2] = -1 * sin(reinterpret_cast<double>(angle));
        matrix[2][1] = sin(reinterpret_cast<double>(angle));
        matrix[2][2] = cos(reinterpret_cast<double>(angle));
        matrix[3][3] = 1;
        break;
      case Axis::Y:
        matrix[0][0] = cos(reinterpret_cast<double>(angle));
        matrix[0][2] = sin(reinterpret_cast<double>(angle));
        matrix[1][1] = 1;
        matrix[2][0] = -1 * sin(reinterpret_cast<double>(angle));
        matrix[2][2] = cos(reinterpret_cast<double>(angle));
        matrix[3][3] = 1;
        break;
    }
  }

  static ZVector<cols, T> ApplyTransform(const ZMatrix<rows, cols, T>& matrix, const ZVector<cols, T>& domain) {
    ZVector<cols, T> codomainResult;

    for (std::size_t row = 0; row < rows; row++) {
      codomainResult[row] = domain * matrix[row];
    }

    return codomainResult;
  }

  private:
  ZVector<cols, T> mData[rows];
};
}

#endif
