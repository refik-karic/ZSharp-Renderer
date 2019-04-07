#include "Constants.h"
#include "IndexBuffer.h"

namespace ZSharp {

IndexBuffer::IndexBuffer(std::size_t size) :
  mAllocatedSize(size* Constants::MAX_INDICIES_AFTER_CLIP),
  mData(size * Constants::MAX_INDICIES_AFTER_CLIP)
{

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
  mWorkingSize = rhs.mWorkingSize;
  mClipLength = rhs.mClipLength;
}

std::size_t IndexBuffer::operator[](std::size_t index) const {
  return mData[index];
}

std::size_t& IndexBuffer::operator[](std::size_t index) {
  return mData[index];
}

std::size_t IndexBuffer::GetWorkingSize() const {
  return mWorkingSize;
}

void IndexBuffer::SetWorkingSize(std::size_t size) {
  mWorkingSize = size;
}

void IndexBuffer::CopyData(const std::size_t* data, std::size_t index, std::size_t length) {
  // memcpy inside here to avoid having to expose raw pointers to the underlying buffer.
  std::memcpy(mData.data() + index, data, length * sizeof(std::size_t));
}

void IndexBuffer::Clear() {
  std::memset(mData.data(), 0, mData.size() * sizeof(std::size_t));
  mClipLength = 0;
  mWorkingSize = 0;
}

std::size_t IndexBuffer::GetClipLength() const {
  return mClipLength;
}

}
