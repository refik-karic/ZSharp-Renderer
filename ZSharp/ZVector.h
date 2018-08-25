#ifndef ZVECTOR_H
#define ZVECTOR_H

#include <cstdint>
#include <cstring>

namespace ZSharp {

/// <summary>
/// Generic vector containg an arbitrary amount of elements. 
/// </summary>
template<typename T>
class ZVector {
  public:
  /// <summary>
  /// Pointer to the starting block of elements. 
  /// </summary>
  T* mData;

  /// <summary>
  /// Number of elements contained in this vector. 
  /// </summary>
  const uint32_t mSize;

  /// <summary>
  /// Create a new vector with the given dimensions.
  /// </summary>
  /// <param name='dims'>The number of "dimensions" this vector will store.</param>
  ZVector(uint32_t dims) : 
    mData(new T[dims]),
    mSize(dims)
  {
    ZVector<T>::Clear(*this);
  }

  /// <summary>
  /// Copy an exisiting vector to produce an identical one. 
  /// </summary>
  /// <param name='copy'>An exisiting vector to generate a copy from.</param>
  ZVector(const ZVector<T>& copy) :
    mSize(copy.mSize)
  {
    // Protect against self assignment.
    if (this == &copy) {
      return;
    }

    // Copy the data over to the class.
    mData = new T[copy.mSize];
    *this = copy;
  }

  /// <summary>
  /// Free the memory containing the data elements. 
  /// </summary>
  ~ZVector() {
    delete[] mData;
  }

  /// <summary>
  /// Copy all of the data elements from the RHS to the LHS. 
  /// </summary>
  /// <param name='vector'>A vector to copy data elements from.</param>
  void operator=(const ZVector<T>& vector) {
    // Don't waste time performing assignment on it's own data.
    // TODO: Look into whether branching here, at least for vectors in R3 or R4, is faster than actually performing the operations.
    // Most of the time this will be false so maybe the branch predictor will pick up on it.
    // However, the data will be immediately available in L(x) cache so it should be investigated further.
    if (this == &vector) {
      return;
    }

    // TODO: Look into using type traits to implement type-specific `memcpy` using SSE(x) or AVX(x).
    // For more information about how to implement (type traits) that, see:
    // Scott Meyers Effective C++ Third Edition, Item 47: p.226.
    // std::memcpy(this->mData, vector.mData, vector.mSize);

    for (uint32_t i = 0; i < vector.mSize; i++) {
      this->mData[i] = vector.mData[i];
    }
  }

  /// <summary>
  /// Add two vectors. 
  /// </summary>
  /// <param name='vector'>RHS vector to perform addition with.</param>
  /// <returns>A new resultant vector, argument vectors remain unchanged.</returns>
  ZVector<T> operator+(const ZVector<T>& vector) {
    ZVector<T> tmpVec(vector.mSize);
    
    for (uint32_t i = 0; i < vector.mSize; i++) {
      tmpVec.mData[i] = mData[i] + vector.mData[i];
    }

    return tmpVec;
  }

  /// <summary>
  /// Subtract two vectors.
  /// </summary>
  /// <param name='vector'>RHS vector to perform subtraction with.</param>
  /// <returns>A new resultant vector, argument vectors remain unchanged.</returns>
  ZVector<T> operator-(const ZVector<T>& vector) {
    ZVector<T> tmpVec(vector.mSize);

    for (uint32_t i = 0; i < vector.mSize; i++) {
      tmpVec.mData[i] = mData[i] - vector.mData[i];
    }

    return tmpVec;
  }

  /// <summary>
  /// Scalar multiply a vector.
  /// </summary>
  /// <param name='scalar'>The scalar to multiply through with.</param>
  /// <returns>A new resultant vector, argument vectors remain unchanged.</returns>
  ZVector<T> operator*(const T scalar) {
    ZVector<T> tmpVec(mSize);

    for (uint32_t i = 0; i < mSize; i++) {
      tmpVec.mData[i] = mData[i] * scalar;
    }

    return tmpVec;
  }

  /// <summary>
  /// Dot product between two vectors.
  /// </summary>
  /// <param name='vector'>RHS vector to dot against.</param>
  /// <returns>The resulting value, argument vectors remain unchanged.</returns>
  T operator*(const ZVector<T>& vector) {
    T result{};

    for (uint32_t i = 0; i < vector.mSize; i++) {
      result += (mData[i] * vector.mData[i]);
    }

    return result;
  }

  /// <summary>
  /// Set all of the data elements in the vector to their type-equivalent of 0.
  /// </summary>
  /// <param name='vector'>Vector to clear.</param>
  static void Clear(const ZVector<T>& vector) {
    T zero{};

    for (uint32_t i = 0; i < vector.mSize; i++) {
      vector.mData[i] = zero;
    }
  }
};
}
#endif
