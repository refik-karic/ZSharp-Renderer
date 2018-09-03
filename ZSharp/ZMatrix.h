#ifndef ZMATRIX_H
#define ZMATRIX_H

#include <cmath>

#include "ZVector.h"

namespace ZSharp {

/// <summary>
/// Generic matrix containing an arbitrary amount of rows and columns.
/// </summary>
template <uint32_t rows, uint32_t cols , typename T>
class ZMatrix {
  public:
  /// <summary>
  /// Used to specify an axis of rotation.
  /// </summary>
  enum class Axis {
    TWO_DIMENSIONS,
    X,
    Y,
    Z
  };

  /// <summary>
  /// Constructs a new matrix given the dimensions.
  /// </summary>
  ZMatrix() {

  }

  /// <summary>
  /// Create a copy of an existing matrix.
  /// </summary>
  /// <param name='copy'>The matrix to copy from.</param>
  ZMatrix(const ZMatrix<rows, cols, T>& copy) {
    // Self copy guard.
    if (this == &copy) {
      return;
    }

    *this = copy;
  }

  /// <summary>
  /// Perform a deep copy of the elements from the argument matrix into this matrix.
  /// </summary>
  /// <param name='matrix'>The matrix to copy from.</param>
  void operator=(const ZMatrix<rows, cols, T>& matrix) {
    // Self assignment guard.
    if (this == &matrix) {
      return;
    }

    for (uint32_t row(0); row < rows; row++) {
      mData[row] = matrix[row];
    }
  }

  /// <summary>
  /// Get the vector located at the request row.
  /// <summary>
  /// <param name='index'>Index into the matrix.</param>
  /// <returns>A vector located at the requested index.</returns>
  ZVector<cols, T>& operator[](const uint32_t index) {
    return mData[index];
  }

  /// <summary>
  /// Perform scalar multipltication on the matrix and return a new matrix with the result.
  /// </summary>
  /// <param name='scalar'>The scalar to multiply through with.</param>
  /// <returns>A new matrix containing the result.</returns>
  ZMatrix<rows, cols, T> operator*(T scalar) {
    ZMatrix<rows, cols, T> result;

    for (uint32_t row(0); row < rows; row++) {
      result[row] = mData[row] * scalar;
    }

    return result;
  }

  /// <summary>
  /// Multiply the current matrix against the argument and return the result.
  /// </summary>
  /// <param name='matrix'>The matrix to multiply against.</param>
  /// <returns>A new matrix containing the result of the multiplication.</param>
  ZMatrix<rows, cols, T> operator*(const ZMatrix<rows, cols, T>& matrix) {
    ZMatrix<rows, cols, T> result;

    // Transpose the right hand side to allow a simple dot product on each row.
    ZMatrix<rows, cols, T> rhsTranspose(ZMatrix<rows, cols, T>::Transpose(matrix));

    for (uint32_t row(0); row < rows; row++) {
      for (uint32_t col(0); col < cols; col++) {
        result[row][col] = mData[row] * rhsTranspose[col];
      }
    }

    return result;
  }

  /// <summary>
  /// Assigns the given matrix to its identity.
  /// </summary>
  /// <param name='matrix'>The matrix to assign an identity to.</param>
  static void Identity(ZMatrix<rows, cols, T>& matrix) {
    T zero{};
    T one{1};

    for (uint32_t row(0); row < rows; row++) {
      for (uint32_t col(0); col < cols; col++) {
        if (row == col) {
          matrix[row][col] = one;
        }
        else {
          matrix[row][col] = zero;
        }
      }
    }
  }
  
  /// <summary>
  /// Clear all of the values of the matrix to their type-specific zero equivalent.
  /// </summary>
  /// <param name='matrix'>The matrix to clear.</param>
  static void Clear(ZMatrix<rows, cols, T>& matrix) {
    for (uint32_t row(0); row < rows; row++) {
      ZVector<cols, T>::Clear(matrix[row]);
    }
  }

  /// <summary>
  /// Get the transpose of a given matrix.
  /// </summary>
  /// <param name='matrix'>The matrix to transpose.</param>
  /// <returns>A new matrix representing the transposed version of the argument matrix.</returns>
  static ZMatrix<rows, cols, T> Transpose(const ZMatrix<rows, cols, T>& matrix) {
    ZMatrix<rows, cols, T> result;

    for (uint32_t row(0); row < rows; row++) {
      for (uint32_t col(0); col < cols; col++) {
        result[row][col] = matrix[col][row];
      }
    }

    return result;
  }

  /// <summary>
  /// Apply a translation to the given matrix.
  /// Any subsequent calls to this method will override the previous translation if the vector length is >= the previous.
  /// </summary>
  /// <param name='matrix'>The matrix to apply the translation to.</param>
  /// <param name='translation'>A translation vector containing the amount to translate in each dimension.</param>
  static void SetTranslation(ZMatrix<rows, cols, T>& matrix, const ZVector<cols, T>& translation) {
    uint32_t lastColumn = cols - 1;
    for (uint32_t row(0); row < rows; row++) {
      matrix[row][lastColumn] = translation[row];
    }
  }

  /// <summary>
  /// Apply a scale to the given matrix.
  /// </summary>
  /// <param name='matrix'>The matrix to apply the scale to.</param>
  /// <param name='scale'>The amount to scale in each dimension.</param>
  static void SetScale(ZMatrix<rows, cols, T>& matrix, const ZVector<cols, T>& scale) {
    for (uint32_t row(0); row < rows; row++) {
      matrix[row][row] = scale[row];
    }
  }

  /// <summary>
  /// Apply a rotation to the given matrix.
  /// </summary>
  /// <param name='matrix'>The matrix to apply the rotation to.</param>
  /// <param name='angle'>The angle of rotation.</param>
  /// <param name='axis>The axis of rotation.</param>
  static void SetRotation(ZMatrix<rows, cols, T>& matrix, const T angle, const Axis axis) {
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

  /// <summary>
  /// Apply a linear transformation to map the domain to the codomain.
  /// </summary>
  /// <param name='matrix'>A linear transformation.</param>
  /// <param name='domain'>A vector in the domain to be mapped.</param>
  /// <returns>The codomain result.</returns>
  static ZVector<cols, T> ApplyTransform(const ZMatrix<rows, cols, T>& matrix, const ZVector<cols, T>& domain) {
    ZVector<cols, T> codomainResult;

    for (uint32_t row(0); row < rows; row++) {
      codomainResult[row] = domain * matrix[row];
    }

    return codomainResult;
  }

  private:
  /// <summary>
  /// The block of data stored in this matrix.
  /// </summary>
  ZVector<cols, T> mData[rows];
};
}

#endif
