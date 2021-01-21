#pragma once

#include <cstddef>

#include "UtilMath.h"
#include "Vec3.h"

namespace ZSharp {

template<typename T>
class Vec4 final {
  public:
  Vec4() {
    Vec4<T>::Clear(*this);
  }

  Vec4(const Vec4<T>& copy) {
    if (this != &copy) {
      *this = copy;
    }
  }

  Vec4(const Vec3<T>& copy) {
    *this = copy;
  }

  void operator=(const Vec3<T>& vector) {
    for (std::size_t i = 0; i < 3; ++i) {
      mData[i] = vector[i];
    }
  }

  void operator=(const Vec4<T>& vector) {
    if (this == &vector) {
      return;
    }
    
    for (std::size_t i = 0; i < Elements; ++i) {
      mData[i] = vector[i];
    }
  }

  bool operator==(const Vec4<T>& vector) const {
    // TODO: memcmp might be faster?
    bool result = true;

    for (std::size_t i = 0; (i < Elements) && result; i++) {
      result = result && (mData[i] == vector[i]);
    }

    return result;
  }

  T* operator*() const {
    return mData;
  }

  T operator[](std::size_t index) const {
    return mData[index];
  }

  T& operator[](std::size_t index) {
    return mData[index];
  }

  Vec4<T> operator+(const Vec4<T>& vector) const {
    Vec4<T> result;
    
    for (std::size_t i = 0; i < Elements; i++) {
      result[i] = mData[i] + vector[i];
    }

    return result;
  }

  Vec4<T> operator-(const Vec4<T>& vector) const {
    Vec4<T> result;

    for (std::size_t i = 0; i < Elements; i++) {
      result[i] = mData[i] - vector[i];
    }

    return result;
  }

  Vec4<T> operator*(T scalar) const {
    Vec4<T> result;

    for (std::size_t i = 0; i < Elements; i++) {
      result[i] = mData[i] * scalar;
    }

    return result;
  }

  T operator*(const Vec4<T>& vector) {
    T result{};

    for (std::size_t i = 0; i < Elements; i++) {
      result += (mData[i] * vector[i]);
    }

    return result;
  }

  T operator*(const Vec4<T>& vector) const {
    T result{};

    for (std::size_t i = 0; i < Elements; i++) {
      result += (mData[i] * vector[i]);
    }

    return result;
  }

  static T Length(const Vec4<T>& vector) {
    return NewtonRaphsonSqrt(vector * vector);
  }

  static void Normalize(Vec4<T>& vector) {
    T invSqrt(1 / Length(vector));

    for (std::size_t i = 0; i < Elements; i++) {
      vector[i] *= invSqrt;
    }
  }

  static void Homogenize(Vec4<T>& vector, std::size_t element) {
    T divisor(vector[element]);

    for (std::size_t i = 0; i <= element; i++) {
      vector[i] /= divisor;
    }
  }

  static void Clear(Vec4<T>& vector) {
    T zero{};

    for (std::size_t i = 0; i < Elements; i++) {
      vector[i] = zero;
    }
  }

  private:
  static const std::size_t Elements = 4;
  T mData[Elements];
};

}
