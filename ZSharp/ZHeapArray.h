#ifndef ZHEAPARRAY_H
#define ZHEAPARRAY_H

#include <cstddef>

namespace ZSharp {

template <typename T>
class ZHeapArray {
  public:
  ZHeapArray() {

  }

  ZHeapArray(std::size_t size) :
    mSize(size),
    mData(new T[size])
  {
  }

  ~ZHeapArray() {
    if (mSize > 0) {
      delete[] mData;
    }
  }

  ZHeapArray(const ZHeapArray<T>& copy) {
    if (this == &copy) {
      return;
    }

    *this = copy;
  }
  
  void operator=(const ZHeapArray<T>& rhs) {
    if (this == &rhs) {
      return;
    }

    Resize(rhs.Size());
    CopyData(rhs.Data(), rhs.Size());
  }

  T operator[](std::size_t index) const {
    return mData[index];
  }

  T& operator[](std::size_t index) {
    return mData[index];
  }

  T* Data() {
    return mData;
  }

  const T* Data() const {
    return mData;
  }

  std::size_t Size() const {
    return mSize;
  }

  void Resize(std::size_t size) {
    if (mSize > 0) {
      delete[] mData;
    }

    mSize = size;
    mData = new T[size];
  }

  void CopyData(const T* data, std::size_t length) {
    for (std::size_t i = 0; i < length; ++i) {
      mData[i] = data[i];
    }
  }

  private:
  T* mData;
  std::size_t mSize = 0;
};

}

#endif
