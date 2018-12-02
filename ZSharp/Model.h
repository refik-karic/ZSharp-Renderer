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

    mData = rhs.GetMeshData();
  }

  Mesh<T>& operator[](std::size_t index) {
    return mData[index];
  }

  std::size_t MeshCount() {
    return mData.Size();
  }

  ZHeapArray<Mesh<T>>& GetMeshData() {
    return mData;
  }

  const ZHeapArray<Mesh<T>>& GetMeshData() const {
    return mData;
  }

  private:
  ZHeapArray<Mesh<T>> mData;
};

}

#endif
