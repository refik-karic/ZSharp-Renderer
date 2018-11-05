﻿#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <vector>

#include "ZVector.h"

namespace ZSharp {

struct Triangle {
  uint32_t index[3];
};

// TODO: Figure out why this template exists since it isn't used for anything.
template <typename T>
struct Mesh {
  std::list<Triangle> triangleList;
};

template <typename T>
struct Model {
  std::list<Mesh<T>> meshList;
  std::vector<ZVector<3, T>>* verticies;
};

}

#endif
