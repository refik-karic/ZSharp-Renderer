#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <string>
#include <vector>

namespace ZSharp {

template<typename T>
class VertexBuffer {
  public:

  VertexBuffer(std::size_t size, std::size_t stride) :
    mData(size * 2),
    mRequestedSize(size),
    mStride(stride)
  {

  }

  VertexBuffer(const VertexBuffer<T>& rhs) {
    if (this != &rhs) {
      *this = rhs;
    }
  }

  void operator=(const VertexBuffer<T>& rhs) {
    if (this == &rhs) {
      return;
    }

    mData = rhs.mData;
  }

  T operator[](std::size_t index) const {
    return mData[index];
  }

  T& operator[](std::size_t index) {
    return mData[index];
  }

  std::size_t GetRequestedSize() const {
    return mRequestedSize;
  }

  std::size_t GetTotalSize() const {
    return mData.size();
  }

  std::size_t GetWorkingSize() const {
    return mWorkingSize;
  }

  void SetWorkingSize(std::size_t size) {
    mWorkingSize = size;
  }

  std::size_t GetStride() const {
    return mStride;
  }

  void CopyData(const T* data, std::size_t index, std::size_t length) {
    // memcpy inside here to avoid having to expose raw pointers to the underlying buffer.
    std::memcpy(mData.data() + index, data, length * sizeof(T));
  }

  T* GetData() {
    return mData.data();
  }

  const T* GetData() const {
    return mData.data();
  }

  void Clear() {
    std::memset(mData.data(), 0, mData.size() * sizeof(T));
  }

  private:
  std::vector<T> mData;
  std::size_t mRequestedSize = 0;
  std::size_t mWorkingSize = 0;
  std::size_t mStride = 0;
};

}

#endif
