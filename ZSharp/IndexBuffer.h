#pragma once

#include <vector>

namespace ZSharp {

class IndexBuffer {
  public:

  IndexBuffer(std::size_t size);

  IndexBuffer(const IndexBuffer& rhs);

  void operator=(const IndexBuffer& rhs);

  std::size_t operator[](std::size_t index) const;

  std::size_t& operator[](std::size_t index);

  std::size_t GetTotalSize() const;

  std::size_t GetRequestedSize() const;

  std::size_t GetWorkingSize() const;

  void SetWorkingSize(std::size_t size);

  void CopyData(const std::size_t* data, std::size_t index, std::size_t length);

  void Clear();

  private:
  static constexpr std::size_t MAX_INDICIES_AFTER_CLIP = 4;

  std::vector<std::size_t> mData;
  std::size_t mRequestedSize = 0;
  std::size_t mWorkingSize = 0;
};

}
