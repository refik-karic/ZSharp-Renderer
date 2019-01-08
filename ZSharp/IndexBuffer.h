#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <vector>

namespace ZSharp {

class IndexBuffer {
  public:

  IndexBuffer(std::size_t size) :
    mData(size * 4)
  {

  }

  IndexBuffer(const IndexBuffer& rhs) {
    if (this != &rhs) {
      *this = rhs;
    }
  }

  void operator=(const IndexBuffer& rhs) {
    if (this == &rhs) {
      return;
    }

    mData = rhs.mData;
  }

  std::size_t operator[](std::size_t index) const {
    return mData[index];
  }

  std::size_t& operator[](std::size_t index) {
    return mData[index];
  }

  std::size_t GetSize() const {
    return mData.size();
  }

  private:
  std::vector<std::size_t> mData;
};

}

#endif
