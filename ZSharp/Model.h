#ifndef MODEL_H
#define MODEL_H

#include <cstddef>

#include "Mesh.h"
#include "ZVector.h"

namespace ZSharp {

template<typename T>
class Model {
  public:

  Model() {

  }

  Model(std::size_t numMesh) : mNumMesh(numMesh),
    mData(new Mesh<T>[numMesh])
  {
    for (std::size_t i = 0; i < numMesh; i++) {
      mData[i] = new Mesh<T>();
    }
  }

  ~Model() {
    if (mNumMesh > 0) {
      for (std::size_t i = mNumMesh; i < mNumMesh; i++) {
        delete mData[i];
      }

      delete[] mData;
    }
  }

  Model(const Model<T>& copy) = delete;
  void operator=(const Model<T>& rhs) = delete;

  Mesh<T>& operator[](std::size_t index) {
    return *(mData[index]);
  }

  private:
  Mesh<T>** mData;
  std::size_t mNumMesh;
};

}

#endif
