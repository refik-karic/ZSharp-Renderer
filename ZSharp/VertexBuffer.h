#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>

namespace ZSharp {

template<typename T>
class VertexBuffer {
  public:

  VertexBuffer(std::size_t size, std::size_t stride) :
    mData(size * 2),
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

  std::size_t GetSize() const {
    return mData.size();
  }

  std::size_t GetStride() const {
    return mStride;
  }

  private:
  std::vector<T> mData;
  std::size_t mStride = 0;
};

}

#endif
