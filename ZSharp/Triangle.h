#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cstddef>
#include "ZVector.h"

namespace ZSharp {

template<typename T>
class Triangle {
  public:

  Triangle() {

  }

  Triangle(std::size_t p1, std::size_t p2, std::size_t p3) :
    mIndicies({p1, p2, p3})
  { }

  Triangle(const Triangle<T>& copy) {
    // Self copy guard.
    if (this == &copy) {
      return;
    }

    // Perform a deep copy.
    *this = copy;
  }

  void operator=(const Triangle<T>& rhs) {
    // Self assignment guard.
    if (this == &rhs) {
      return;
    }

    mIndicies[0] = rhs.mIndicies[0];
    mIndicies[1] = rhs.mIndicies[1];
    mIndicies[2] = rhs.mIndicies[2];
  }

  void SetPoint(std::size_t point, std::size_t index) {
    mIndicies[point] = index;
  }

  /*ZVector<3, T>& operator[](std::size_t index) {
    return mData[index];
  }*/

  private:
  std::size_t mIndicies[3];
};

}

#endif
