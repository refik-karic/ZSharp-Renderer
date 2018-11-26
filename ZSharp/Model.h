#ifndef MODEL_H
#define MODEL_H

#include <cstddef>

#include "Mesh.h"
#include "ZHeapArray.h"
#include "ZVector.h"

namespace ZSharp {

template<typename T>
class Model {
  public:

  Model() {}

  Model(std::size_t numMesh) : mData(numMesh) {

  }

  Model(const Model<T>& copy) {
    if (this == &copy) {
      return;
    }

    *this = copy;
  }

  void operator=(const Model<T>& rhs) {
    if (this == &rhs) {
      return;
    }

    mData = rhs.mData;
  }

  Mesh<T>& operator[](std::size_t index) {
    return mData[index];
  }

  std::size_t MeshCount() {
    return mData.Size();
  }

  private:
  ZHeapArray<Mesh<T>> mData;
};

}

#endif
