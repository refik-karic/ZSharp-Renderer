#ifndef MESH_H
#define MESH_H

#include <cstddef>

#include "Triangle.h"

namespace ZSharp {

template<typename T>
class Mesh {
  public:

  Mesh() {

  }

  Mesh(std::size_t dataSize, std::size_t numPrimitives) : 
    mRawDataSize(dataSize),
    mNumPrimitives(numPrimitives),
    mRawData(new T[dataSize]),
    mPrimData(new Triangle<T>[numPrimitives])
  {
    for (std::size_t i = 0; i < numPrimitives; i++) {
      mData[i] = new Triangle<T>();
    }
  }

  ~Mesh() {
    if (mRawDataSize > 0) {
      delete[] mRawData;
    }

    if (mNumPrimitives > 0) {
      for (std::size_t i = 0; i < mNumPrimitives; i++) {
        delete mData[i];
      }
      delete[] mData;
    }
  }

  Mesh(const Mesh<T>& copy) = delete;
  void operator=(const Mesh<T>& rhs) = delete;

  Triangle<T>& operator[](std::size_t index) {
    return *(mData[index]);
  }

  // TODO: Come back to this later and find a better way to do this.
  // Need to handle the raw array as well as the triangle indicies somehow.
  void SetData(const Triangle<T>** data, std::size_t length) {
    if (mNumPrimitives == 0) {
      mNumPrimitives = length;
      mData = new Triangle<T>[length];
    
      for (std::size_t i = 0; i < length && i < mNumPrimitives; i++) {
        mData[i] = new Triangle<T>();
        mData[i] = *(data[i]);
      }
    }
    else {
      for (std::size_t i = 0; i < length && i < mNumPrimitives; i++) {
        mData[i] = *(data[i]);
      }
    }
  }

  private:
  // Backing buffer for all the geometry in this mesh.
  T* mRawData;

  // Each triangle with indicies into the raw data.
  Triangle<T>** mPrimData;

  std::size_t mRawDataSize;
  std::size_t mNumPrimitives = 0;
};
}

#endif
