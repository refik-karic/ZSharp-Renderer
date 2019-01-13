#include "IndexBuffer.h"

namespace ZSharp {

IndexBuffer::IndexBuffer(std::size_t size) :
  mData(size * 4) {

}

IndexBuffer::IndexBuffer(const IndexBuffer& rhs) {
  if (this != &rhs) {
    *this = rhs;
  }
}

void IndexBuffer::operator=(const IndexBuffer& rhs) {
  if (this == &rhs) {
    return;
  }

  mData = rhs.mData;
}

std::size_t IndexBuffer::operator[](std::size_t index) const {
  return mData[index];
}

std::size_t& IndexBuffer::operator[](std::size_t index) {
  return mData[index];
}

std::size_t IndexBuffer::GetSize() const {
  return mData.size();
}

void IndexBuffer::CopyData(const std::size_t* data, std::size_t index, ::size_t length) {
  // memcpy inside here to avoid having to expose raw pointers to the underlying buffer.
  std::memcpy(mData.data() + index, data, length);
}

}
