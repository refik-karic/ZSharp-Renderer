#ifndef ZVECTOR_H
#define ZVECTOR_H

#include <cstdint>
#include <cstring>

#include "UtilMath.h"

namespace ZSharp {

// Thank you to Joey De Vries for his C++ example on GitHub of how to create fixed size stack allocated generic vectors/matricies.
// https://github.com/JoeyDeVries/Cell
// https://github.com/JoeyDeVries/Cell/blob/master/math/linear_algebra/vector.h#L24
// I was scratching my head and doing it wrong the whole time trying to manage my memory very efficiently and supporting stack allocated data by providing an overloaded constructor that takes a pointer to a block of memory.
// The solution seems so obvious now.
// One drawback is that things like multiplying matricies no longer supports different sizes unless you add another parameter (template or function).
// Another major benefit is the ability for the compiler to decide at compile time the amount of loop iterations for common operations like addition and unroll them for you without you having to do it yourself.

/// <summary>
/// Generic vector containg an arbitrary amount of elements. 
/// </summary>
template<uint32_t elements, typename T>
class ZVector {
  public:
  /// <summary>
  /// Create a new vector with the given dimensions and clears its contents.
  /// </summary>
  ZVector() {
    ZVector<elements, T>::Clear(*this);
  }

  /// <summary>
  /// Copy an exisiting vector to produce an identical one. 
  /// </summary>
  /// <param name='copy'>An exisiting vector to generate a copy from.</param>
  ZVector(const ZVector<elements, T>& copy) {
    // Self copy guard.
    if (this == &copy) {
      return;
    }

    // Perform a deep copy.
    *this = copy;
  }

  /// <summary>
  /// Perform a deep copy of the elements from the argument vector into this vector. 
  /// </summary>
  /// <param name='vector'>A vector to copy data elements from.</param>
  void operator=(const ZVector<elements, T>& vector) {
    // Self assignment guard.
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

    for (uint32_t i(0); i < elements; i++) {
      mData[i] = vector[i];
    }
  }

  /// <summary>
  /// Get the element at the given index.
  /// This does not perform bounds checking.
  /// </summary>
  /// <param name='index'>Index into the vector.</param>
  /// <returns>The element at the requested index.</returns>
  T operator[](const uint32_t index) const {
    return mData[index];
  }

  /// <summary>
  /// Get the address of the element at the given index.
  /// This does not perform bounds checking.
  /// </summary>
  /// <param name='index'>Index into the vector.</param>
  /// <returns>The address of the element at the requested index.</returns>
  T& operator[](const uint32_t index) {
    return mData[index];
  }

  /// <summary>
  /// Add two vectors. 
  /// </summary>
  /// <param name='vector'>RHS vector to perform addition with.</param>
  /// <returns>A new resultant vector, argument vectors remain unchanged.</returns>
  ZVector<elements, T> operator+(const ZVector<elements, T>& vector) {
    ZVector<elements, T> result;
    
    for (uint32_t i(0); i < elements; i++) {
      result[i] = mData[i] + vector[i];
    }

    return result;
  }

  /// <summary>
  /// Subtract two vectors.
  /// </summary>
  /// <param name='vector'>RHS vector to perform subtraction with.</param>
  /// <returns>A new resultant vector, argument vectors remain unchanged.</returns>
  ZVector<elements, T> operator-(const ZVector<elements, T>& vector) {
    ZVector<elements, T> result;

    for (uint32_t i(0); i < elements; i++) {
      result[i] = mData[i] - vector[i];
    }

    return result;
  }

  /// <summary>
  /// Scalar multiply a vector.
  /// </summary>
  /// <param name='scalar'>The scalar to multiply through with.</param>
  /// <returns>A new resultant vector, argument vectors remain unchanged.</returns>
  ZVector<elements, T> operator*(const T scalar) {
    ZVector<elements, T> result;

    for (uint32_t i(0); i < elements; i++) {
      result[i] = mData[i] * scalar;
    }

    return result;
  }

  /// <summary>
  /// Dot product between two vectors.
  /// </summary>
  /// <param name='vector'>RHS vector to dot against.</param>
  /// <returns>The resulting value, argument vectors remain unchanged.</returns>
  T operator*(const ZVector<elements, T>& vector) {
    T result{};

    for (uint32_t i(0); i < elements; i++) {
      result += (mData[i] * vector[i]);
    }

    return result;
  }

  /// <summary>
  /// Calculate the cross product between two vectors and return the resultant vector.
  /// 
  /// NOTE: The cross product in ONLY defined in R3.
  /// </summary>
  /// <param name="v1">The first vector.</param>
  /// <param name="v2">The second vector.</param>
  /// <returns>A vector that is orthogonal to both argument vectors.</returns>
  static ZVector<elements, T> Cross(const ZVector<elements, T>& v1, const ZVector<elements, T>& v2) {
    ZVector<elements, T> result;

    result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    return result;
  }

  /// <summary>
  /// Calculate the length of the vector and return the value.
  /// </summary>
  /// <param name='vector'>The vector to calculate the length of.</param>
  /// <returns>A scalar value representing the length of the vector.</returns>
  static T Length(const ZVector<elements, T>& vector) {
    return NewtonRaphsonSqrt((*this) * (*this));
  }

  /// <summary>
  /// Normalize a vector to its unit form in the range of [0,1].
  /// </summary>
  /// <param name='vector'>The vector to normalize.</param>
  static void Normalize(ZVector<elements, T>& vector) {
    T invSqrt(1 / Length(vector));

    for (uint32_t i(0); i < elements; i++) {
      vector[i] *= invSqrt;
    }
  }

  /// <summary>
  /// Convert the given vector to its homogeneous version.
  /// </summary>
  /// <param name='vector'>The vector of elements to homogenize.</param>
  /// <param name='element'>The index of the element to homogenize with.</param>
  static void Homogenize(ZVector<elements, T>& vector, const uint32_t element) {
    T divisor(vector[element]);

    for (uint32_t i(0); i <= element; i++) {
      vector[i] /= divisor;
    }
  }

  /// <summary>
  /// Set all of the data elements in the vector to their type-equivalent of 0.
  /// </summary>
  /// <param name='vector'>Vector to clear.</param>
  static void Clear(ZVector<elements, T>& vector) {
    T zero{};

    for (uint32_t i(0); i < elements; i++) {
      vector[i] = zero;
    }
  }

  private:
  /// <summary>
  /// The block of data stored in this vector.
  /// </summary>
  T mData[elements];
};
}
#endif
