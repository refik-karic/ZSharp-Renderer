#pragma once

#include <cstddef>

#include "UtilMath.h"

namespace ZSharp {

template<typename T>
class Vec3 final {
  public:
  Vec3() {
    Vec3<T>::Clear(*this);
  }

  Vec3(const Vec3<T>& copy) {
    if (this != &copy) {
      *this = copy;
    }
  }

  void operator=(const Vec3<T>& vector) {
    if (this == &vector) {
      return;
    }

    for (std::size_t i = 0; i < Elements; ++i) {
      mData[i] = vector[i];
    }
  }

  bool operator==(const Vec3<T>& vector) const {
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

  Vec3<T> operator+(const Vec3<T>& vector) const {
    Vec3<T> result;

    for (std::size_t i = 0; i < Elements; i++) {
      result[i] = mData[i] + vector[i];
    }

    return result;
  }

  Vec3<T> operator-(const Vec3<T>& vector) const {
    Vec3<T> result;

    for (std::size_t i = 0; i < Elements; i++) {
      result[i] = mData[i] - vector[i];
    }

    return result;
  }

  Vec3<T> operator*(T scalar) const {
    Vec3<T> result;

    for (std::size_t i = 0; i < Elements; i++) {
      result[i] = mData[i] * scalar;
    }

    return result;
  }

  T operator*(const Vec3<T>& vector) {
    T result{};

    for (std::size_t i = 0; i < Elements; i++) {
      result += (mData[i] * vector[i]);
    }

    return result;
  }

  T operator*(const Vec3<T>& vector) const {
    T result{};

    for (std::size_t i = 0; i < Elements; i++) {
      result += (mData[i] * vector[i]);
    }

    return result;
  }

  static Vec3<T> Cross(const Vec3<T>& v1, const Vec3<T>& v2) {
    Vec3<T> result;

    result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    return result;
  }

  static T Length(const Vec3<T>& vector) {
    return NewtonRaphsonSqrt(vector * vector);
  }

  static void Normalize(Vec3<T>& vector) {
    T invSqrt(1 / Length(vector));

    for (std::size_t i = 0; i < Elements; i++) {
      vector[i] *= invSqrt;
    }
  }

  static void Homogenize(Vec3<T>& vector, std::size_t element) {
    T divisor(vector[element]);

    for (std::size_t i = 0; i <= element; i++) {
      vector[i] /= divisor;
    }
  }

  static void Clear(Vec3<T>& vector) {
    T zero{};

    for (std::size_t i = 0; i < Elements; i++) {
      vector[i] = zero;
    }
  }

  private:
  static const std::size_t Elements = 3;
  T mData[Elements];
};

}
