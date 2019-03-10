#pragma once

#include <cstddef>
#include <cstring>

#include <array>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ZVector.h"

namespace ZSharp {

template<typename T, std::size_t size>
class ClipBuffer {
  public:
  
  ClipBuffer() {
    Clear();
  }

  ClipBuffer(const ClipBuffer<T, size>& rhs) {
    if (this != &rhs) {
      *this = rhs;
    }
  }

  void operator=(const ClipBuffer<T, size>& rhs) {
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

  void Clear() {
    std::memset(mData.data(), 0, size * sizeof(T));
  }

  void ClipTriangles(VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer) {
    ZVector<3, T> nextEdge;

    // Right/Positive X.
    nextEdge[0] = static_cast<T>(1);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, nextEdge);

    // Top/Positive Y.
    nextEdge[0] = static_cast<T>(0);
    nextEdge[1] = static_cast<T>(1);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, nextEdge);

    // Left/Negative X.
    nextEdge[0] = static_cast<T>(-1);
    nextEdge[1] = static_cast<T>(0);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, nextEdge);
    
    // Bottom/Negative Y.
    nextEdge[0] = static_cast<T>(0);
    nextEdge[1] = static_cast<T>(-1);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, nextEdge);
    
    // Back/Negative Z.
    nextEdge[1] = static_cast<T>(0);
    nextEdge[2] = static_cast<T>(-1);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, nextEdge);
  }

  private:
  std::array<T, size> mData;

  void SutherlandHodgmanClip(VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer, const ZVector<3, T>& clipEdge) {
    // TODO: Implement the clipping algorithm here.

  }
};

}
