#pragma once

#include <cstddef>

#include "UtilMath.h"

namespace ZSharp {

template<std::size_t elements, typename T>
class ZVector final {
  public:
  ZVector() {
    ZVector<elements, T>::Clear(*this);
  }

  ZVector(const ZVector<elements, T>& copy) {
    if (this != &copy) {
      *this = copy;
    }
  }

  template <std::size_t argElements>
  ZVector(const ZVector<argElements, T>& copy) {
    *this = copy;
  }

  void operator=(const ZVector<elements, T>& vector) {
    if (this == &vector) {
      return;
    }

    for (std::size_t i = 0; i < elements; i++) {
      mData[i] = vector[i];
    }
  }

  template <std::size_t argElements>
  void operator=(const ZVector<argElements, T>& vector) {
    ZVector<elements, T>::Clear(*this);
    
    for (std::size_t i = 0; i < argElements; i++) {
      mData[i] = vector[i];
    }

    T one{1};

    for(std::size_t i = argElements; i < elements; ++i) {
      mData[i] = one;
    }
  }

  bool operator==(const ZVector<elements, T>& vector) const {
    bool result = true;

    for (std::size_t i = 0; (i < elements) && result; i++) {
      result = result && (mData[i] == vector[i]);
    }

    return result;
  }

  T operator[](std::size_t index) const {
    return mData[index];
  }

  T& operator[](std::size_t index) {
    return mData[index];
  }

  ZVector<elements, T> operator+(const ZVector<elements, T>& vector) const {
    ZVector<elements, T> result;
    
    for (std::size_t i = 0; i < elements; i++) {
      result[i] = mData[i] + vector[i];
    }

    return result;
  }

  ZVector<elements, T> operator-(const ZVector<elements, T>& vector) const {
    ZVector<elements, T> result;

    for (std::size_t i = 0; i < elements; i++) {
      result[i] = mData[i] - vector[i];
    }

    return result;
  }

  ZVector<elements, T> operator*(T scalar) const {
    ZVector<elements, T> result;

    for (std::size_t i = 0; i < elements; i++) {
      result[i] = mData[i] * scalar;
    }

    return result;
  }

  template<std::size_t argElements>
  T operator*(const ZVector<argElements, T>& vector) {
    T result{};

    for (std::size_t i = 0; i < argElements; i++) {
      result += (mData[i] * vector[i]);
    }

    return result;
  }

  template<std::size_t argElements>
  T operator*(const ZVector<argElements, T>& vector) const {
    T result{};

    for (std::size_t i = 0; i < argElements; i++) {
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

typedef ZVector<3, float> Vec3f_t;
typedef ZVector<4, float> Vec4f_t;

}
