#ifndef ZVECTOR_H
#define ZVECTOR_H

#include <cstddef>

#include "UtilMath.h"

namespace ZSharp {

// Thank you to Joey De Vries for his C++ example on GitHub of how to create fixed size stack allocated generic vectors/matricies.
// https://github.com/JoeyDeVries/Cell
// https://github.com/JoeyDeVries/Cell/blob/master/math/linear_algebra/vector.h#L24
// I was scratching my head and doing it wrong the whole time trying to manage my memory very efficiently and supporting stack allocated data by providing an overloaded constructor that takes a pointer to a block of memory.
// The solution seems so obvious now.
// One drawback is that things like multiplying matricies no longer supports different sizes unless you add another parameter (template or function).
// Another major benefit is the ability for the compiler to decide at compile time the amount of loop iterations for common operations like addition and unroll them for you without you having to do it yourself.

template<std::size_t elements, typename T>
class ZVector {
  public:

  // Create a new vector with the given dimensions and clears its contents.
  ZVector() {
    ZVector<elements, T>::Clear(*this);
  }

  ZVector(const ZVector<elements, T>& copy) {
    // Self copy guard.
    if (this == &copy) {
      return;
    }

    // Perform a deep copy.
    *this = copy;
  }

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

    for (std::size_t i = 0; i < elements; i++) {
      mData[i] = vector[i];
    }
  }

  template <std::size_t argElements>
  void operator=(const ZVector<argElements, T>& vector) {
    for (std::size_t i = 0; i < argElements; i++) {
      mData[i] = vector[i];
    }
  }

  T operator[](std::size_t index) const {
    return mData[index];
  }

  T& operator[](std::size_t index) {
    return mData[index];
  }

  ZVector<elements, T> operator+(const ZVector<elements, T>& vector) {
    ZVector<elements, T> result;
    
    for (std::size_t i = 0; i < elements; i++) {
      result[i] = mData[i] + vector[i];
    }

    return result;
  }

  ZVector<elements, T> operator-(const ZVector<elements, T>& vector) {
    ZVector<elements, T> result;

    for (std::size_t i = 0; i < elements; i++) {
      result[i] = mData[i] - vector[i];
    }

    return result;
  }

  ZVector<elements, T> operator*(T scalar) {
    ZVector<elements, T> result;

    for (std::size_t i = 0; i < elements; i++) {
      result[i] = mData[i] * scalar;
    }

    return result;
  }

  T operator*(const ZVector<elements, T>& vector) {
    T result{};

    for (std::size_t i = 0; i < elements; i++) {
      result += (mData[i] * vector[i]);
    }

    return result;
  }

  T operator*(const ZVector<elements, T>& vector) const {
    T result{};

    for (std::size_t i = 0; i < elements; i++) {
      result += (mData[i] * vector[i]);
    }

    return result;
  }

  static ZVector<elements, T> Cross(const ZVector<elements, T>& v1, const ZVector<elements, T>& v2) {
    ZVector<elements, T> result;

    result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    return result;
  }

  static T Length(const ZVector<elements, T>& vector) {
    return NewtonRaphsonSqrt(vector * vector);
  }

  static void Normalize(ZVector<elements, T>& vector) {
    T invSqrt(1 / Length(vector));

    for (std::size_t i = 0; i < elements; i++) {
      vector[i] *= invSqrt;
    }
  }

  static void Homogenize(ZVector<elements, T>& vector, std::size_t element) {
    T divisor(vector[element]);

    for (std::size_t i = 0; i <= element; i++) {
      vector[i] /= divisor;
    }
  }

  static void Clear(ZVector<elements, T>& vector) {
    T zero{};

    for (std::size_t i = 0; i < elements; i++) {
      vector[i] = zero;
    }
  }

  private:
  T mData[elements];
};

// Convenience typedefs for frequently used instances of this class.
typedef ZVector<3, float> Vec3f_t;

}
#endif
