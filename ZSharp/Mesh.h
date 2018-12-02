#ifndef MESH_H
#define MESH_H

#include <cstddef>

#include "Triangle.h"
#include "ZHeapArray.h"

namespace ZSharp {

template<typename T>
class Mesh {
  public:

  Mesh() {

  }

  Mesh(std::size_t numVerts, std::size_t numTriangleFaces) {
    mVertTable.Resize(numVerts);
    mTriangleFaceTable.Resize(numTriangleFaces);
  }

  Mesh(const Mesh<T>& copy) {
    if (this == &copy) {
      return;
    }

    *this = copy;
  }
  
  void operator=(const Mesh<T>& rhs) {
    if (this == &rhs) {
      return;
    }

    mVertTable = rhs.GetVertTable();
    mTriangleFaceTable = rhs.GetTriangleFaceTable();
  }

  /*Triangle<T>& operator[](std::size_t index) {
    return *(mData[index]);
  }*/

  void SetData(const T* vertData, std::size_t numVerts, std::size_t numTriangleFaces) {
    mVertTable.Resize(numVerts);
    mVertTable.CopyData(vertData, numVerts);
    mTriangleFaceTable.Resize(numTriangleFaces);
  }

  void SetTriangle(const std::size_t* triangleFaceData, std::size_t index) {
    Triangle<T>& triangle = mTriangleFaceTable[index];
    triangle.SetPoint(0, triangleFaceData[0]);
    triangle.SetPoint(1, triangleFaceData[1]);
    triangle.SetPoint(2, triangleFaceData[2]);
  }

  ZHeapArray<T>& GetVertTable() {
    return mVertTable;
  }

  const ZHeapArray<T>& GetVertTable() const {
    return mVertTable;
  }

  ZHeapArray<Triangle<T>>& GetTriangleFaceTable() {
    return mTriangleFaceTable;
  }

  const ZHeapArray<Triangle<T>>& GetTriangleFaceTable() const {
    return mTriangleFaceTable;
  }

  private:
  ZHeapArray<T> mVertTable;
  ZHeapArray<Triangle<T>> mTriangleFaceTable;
};
}

#endif
