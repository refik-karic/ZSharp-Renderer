#ifndef ZVECTOR_H
#define ZVECTOR_H

#include <cstdint>
#include <cstring>

namespace ZSharp {
template<typename T>
class ZVector {
  public:
  T* mData;
  uint32_t mSize;

  ZVector(uint32_t dims) : 
    mData(new T[dims]),
    mSize(dims)
  {
    ZVector<T>::Clear(*this);
  }

  ~ZVector() {
    delete[] mData;
  }

  static void Clear(const ZVector<T>& vector) {
    static T zero;

    for (uint32_t i = 0; i < vector.mSize; i++) {
      vector.mData[i] = zero;
    }
  }
};
}
#endif
