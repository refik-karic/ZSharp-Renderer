#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <vector>

namespace ZSharp {

class IndexBuffer {
  public:

  IndexBuffer(std::size_t size);

  IndexBuffer(const IndexBuffer& rhs);

  void operator=(const IndexBuffer& rhs);

  std::size_t operator[](std::size_t index) const;

  std::size_t& operator[](std::size_t index);

  std::size_t GetSize() const;

  void CopyData(const std::size_t* data, std::size_t index, ::size_t length);

  private:
  std::vector<std::size_t> mData;
};

}

#endif
