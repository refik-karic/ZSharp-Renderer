#pragma once

#include <vector>

#include "Constants.h"
#include "Triangle.h"

namespace ZSharp {

class IndexBuffer {
  public:

  IndexBuffer(std::size_t size);

  IndexBuffer(const IndexBuffer& rhs);

  void operator=(const IndexBuffer& rhs);

  std::size_t operator[](std::size_t index) const;

  std::size_t& operator[](std::size_t index);

  std::size_t GetWorkingSize() const;

  void SetWorkingSize(std::size_t size);

  void CopyData(const std::size_t* data, std::size_t index, std::size_t length);

  void Clear();

  template<typename T>
  void Append(const Triangle<T>& triangle) {
    if(mWorkingSize + mClipLength + Constants::TRI_VERTS > mAllocatedSize) {
      return;
    }

    const std::size_t* data = reinterpret_cast<const std::size_t*>(&triangle);
    std::memcpy(mData.data() + mWorkingSize + mClipLength, data, Constants::TRI_VERTS * sizeof(std::size_t));
    mClipLength += Constants::TRI_VERTS;
  }

  std::size_t GetClipLength() const;

  private:
  std::vector<std::size_t> mData;
  std::size_t mAllocatedSize = 0;
  std::size_t mWorkingSize = 0;
  std::size_t mClipLength = 0;
};

}
