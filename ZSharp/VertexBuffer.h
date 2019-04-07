#pragma once

#include <string>
#include <vector>

#include "Constants.h"
#include "ZMatrix.h"
#include "ZVector.h"

namespace ZSharp {

template<typename T>
class VertexBuffer {
  public:
  VertexBuffer(std::size_t size, std::size_t stride) :
    mClipLength(0),
    mAllocatedSize((size + (size / Constants::TRI_VERTS)) * Constants::MAX_VERTS_AFTER_CLIP),
    mData((size + (size / Constants::TRI_VERTS)) * Constants::MAX_VERTS_AFTER_CLIP),
    mHomogenizedStride(stride + (stride / Constants::TRI_VERTS))
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
    mWorkingSize = rhs.mWorkingSize;
    mClipLength = rhs.mClipLength;
    mHomogenizedStride = rhs.mHomogenizedStride;
  }

  T operator[](std::size_t index) const {
    return mData[index];
  }

  T& operator[](std::size_t index) {
    return mData[index];
  }

  std::size_t GetTotalSize() const {
    return mData.size();
  }

  std::size_t GetWorkingSize() const {
    return mWorkingSize;
  }

  void SetWorkingSize(std::size_t size) {
    mWorkingSize = size + (size / Constants::TRI_VERTS);
  }

  std::size_t GetStride() const {
    return mHomogenizedStride;
  }

  void CopyData(const T* data, std::size_t index, std::size_t length) {
    // NOTE: It may not necessarily be required to explicitly set the 4th dimension like this but for the purposes of debugging right now, just going to leave this in.
    static constexpr T wDefault{1};
    T* currentIndex = mData.data() + index;
    std::size_t originalStride = mHomogenizedStride - (mHomogenizedStride / Constants::TRI_VERTS);
    for (std::size_t i = 0; i < length; i += originalStride) {
      for (std::size_t j = 0; j < originalStride / Constants::TRI_VERTS; j++) {
        std::memcpy(currentIndex, (data + i) + (j * Constants::TRI_VERTS), Constants::TRI_VERTS * sizeof(T));
        currentIndex[3] = wDefault;
        currentIndex += HOMOGENOUS_3D_SPACE;
      }
    }
  }

  T* GetData(std::size_t index = 0, std::size_t stride = 1) {
    return mData.data() + (index * stride);
  }

  const T* GetData(std::size_t index = 0, std::size_t stride = 1) const {
    return mData.data() + (index * stride);
  }

  void Clear() {
    std::memset(mData.data(), 0, mData.size() * sizeof(T));
    mWorkingSize = 0;
    mClipLength = 0;
  }

  void ApplyTransform(const ZMatrix<4, 4, T>& transform) {
    // TODO: Come back to this later and figure out if the transforms will need to apply to things like texture coordinates as well.
    // Apply the transform to each vertex in the VBO.
    for (std::size_t i = 0; i < mWorkingSize; i += mHomogenizedStride) {
      ZVector<4, T>& vertexVector = *(reinterpret_cast<ZVector<4, T>*>(mData.data() + i));
      vertexVector[3] = static_cast<T>(1);
      vertexVector = ZMatrix<4, 4, T>::ApplyTransform(transform, vertexVector);
    }
  }

  void Append(const T* data, std::size_t length) {
    if(mWorkingSize + mClipLength + length > mAllocatedSize) {
      return;
    }

    std::memcpy(mData.data() + mWorkingSize + mClipLength, data, length * sizeof(T));
    mClipLength += length;
  }

  std::size_t GetClipLength() const {
    return mClipLength;
  }

  private:
  static constexpr std::size_t HOMOGENOUS_3D_SPACE = 4;

  std::vector<T> mData;
  std::size_t mAllocatedSize = 0;
  std::size_t mWorkingSize = 0;
  std::size_t mClipLength = 0;
  std::size_t mHomogenizedStride = 0;
};

}
