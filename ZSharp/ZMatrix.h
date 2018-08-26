#ifndef ZMATRIX_H
#define ZMATRIX_H

#include "ZVector.h"

namespace ZSharp {

/// <summary>
/// Generic matrix containing an arbitrary amount of rows and columns.
/// </summary>
template <typename T>
class ZMatrix {
  public:
  /// <summary>
  /// Amount of rows in this matrix.
  /// </summary>
  const uint32_t mRows;

  /// <summary>
  /// Amount of columns in this matrix.
  /// </summary>
  const uint32_t mCols;

  /// <summary>
  /// Constructs a new matrix given the dimensions.
  /// </summary>
  /// <param name='rows'>The number of rows to create.</param>
  /// <param name='cols'>The number of columns to create.</param>
  ZMatrix(uint32_t rows, uint32_t cols) :
    mRows(rows),
    mCols(cols),
    mData(new ZVector<T>*[rows])
  {
    // Initialize all vectors in the matrix.
    for (uint32_t i(0); i < rows; i++) {
      mData[i] = new ZVector<T>(cols);
    }
  }

  ZMatrix(uint32_t rows, uint32_t cols, T* data) :
    mRows(rows),
    mCols(cols),
    mOwned(false)
  {
    // TODO: Implement a clean way to pull in external data.
    // Also make sure that the destructor works properly with that implementation.
  }

  ZMatrix(const ZMatrix<T>& copy) {
    // Self copy guard.
    if (this == &copy) {
      return;
    }

    // Check whether or not the vector to copy from has data
    if (copy.mOwned) {
      mData = new ZVector<T>*[copy.mRows];

      for (uint32_t row(0); row < copy.rows; row++) {
        mData[row] = new ZVector<T>(copy.mCols);
      }
    }

    *this = copy;
  }

  /// <summary>
  /// Free the memory contained by the individual row vectors and the matrix itself.
  /// </summary>
  ~ZMatrix() {
    if (mOwned) {
      for (uint32_t i(0); i < mRows; i++) {
        delete[] mData[i];
      }

      delete[] mData;
    }
  }

  /// <summary>
  /// Perform a deep copy of the elements from the argument matrix into this matrix.
  /// </summary>
  /// <param name='matrix'>The matrix to copy from.</param>
  void operator=(const ZMatrix<T>& matrix) {
    // Self assignment guard.
    if (this == &matrix) {
      return;
    }

    for (uint32_t row(0); row < mRows; row++) {
      *(mData[row]) = *(matrix[row]);
    }
  }

  /// <summary>
  /// Get the vector located at the request row.
  /// <summary>
  /// <param name='index'>Index into the matrix.</param>
  /// <returns>A pointer to the vector located at the requested index.</returns>
  ZVector<T>& operator[](const uint32_t index) {
    return *mData[index];
  }

  /// <summary>
  /// Assigns the given matrix to its identity.
  /// </summary>
  /// <param name='matrix'>The matrix to assign an identity to.</param>
  static void Identity(ZMatrix<T>& matrix) {
    T zero{};
    T one{1};

    for (uint32_t row(0); row < matrix.mRows; row++) {
      for (uint32_t col(0); col < matrix.mCols; col++) {
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
  static void Clear(const ZMatrix<T>& matrix) {
    for (uint32_t row(0); row < matrix.mRows; row++) {
      ZVector<T>::Clear(matrix[row]);
    }
  }

  private:
  /// <summary>
  /// Pointer to the block of data containing all of the row vectors.
  /// </summary>
  ZVector<T>** mData;

  /// <summary>
  /// Flag indicating whether or not the data contained in this matrix was allocated on construction or passed in externally.
  /// </summary>
  bool mOwned = true;
};
}

#endif
