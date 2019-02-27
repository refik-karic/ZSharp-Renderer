#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <string>
#include <vector>

#include "ZMatrix.h"
#include "ZVector.h"

namespace ZSharp {

// TODO: Make the usage of the VBO more consistent across the application.
// Users want to work in R# but the VBO needs to work in R4!
template<typename T>
class VertexBuffer {
  public:
  VertexBuffer(std::size_t size, std::size_t stride) :
    mData((size + (size / TRI_VERTS)) * MAX_VERTS_AFTER_CLIP),
    mRequestedSize(size + (size / TRI_VERTS)),
    mHomogenizedStride(stride + (stride / TRI_VERTS)),
    mRequestedStride(stride)
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
    mWorkingSize = size + (size / TRI_VERTS);
  }

  std::size_t GetStride() const {
    return mHomogenizedStride;
  }

  void CopyData(const T* data, std::size_t index, std::size_t length) {
    // memcpy inside here to avoid having to expose raw pointers to the underlying buffer.
    //std::memcpy(mData.data() + index, data, length * sizeof(T));
    T wDefault{1};
    T* currentIndex = mData.data() + index;
    for (std::size_t i = 0; i < length; i += mRequestedStride) {
      for (std::size_t j = 0; j < mRequestedStride / TRI_VERTS; j++) {
        std::memcpy(currentIndex, (data + i) + (j * TRI_VERTS), TRI_VERTS * sizeof(T));
        currentIndex[3] = wDefault;
        currentIndex += HOMOGENOUS_3D_SPACE;
      }
    }
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

  void ApplyTransform(const ZMatrix<4, 4, T>& transform) {
    // TODO: Come back to this later and figure out if the transforms will need to apply to things like texture coordinates as well.
    // Apply the transform to each vertex in the VBO.
    for (std::size_t i = 0; i < mWorkingSize; i += mHomogenizedStride) {
      ZVector<4, T> vertexVector;
      vertexVector[3] = static_cast<T>(1);
      vertexVector.LoadRawData(mData.data() + i, TRI_VERTS);
      vertexVector = ZMatrix<4, 4, T>::ApplyTransform(transform, vertexVector);
      vertexVector.StoreRawData(mData.data() + i, TRI_VERTS);
    }
  }

  private:
  static constexpr std::size_t MAX_VERTS_AFTER_CLIP = 3;
  static constexpr std::size_t HOMOGENOUS_3D_SPACE = 4;

  std::vector<T> mData;
  std::size_t mRequestedSize = 0;
  std::size_t mWorkingSize = 0;
  std::size_t mHomogenizedStride = 0;
  std::size_t mRequestedStride = 0;
};

}

#endif
