#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ZVector.h"

namespace ZSharp {

template<typename T>
class Triangle {
  public:

  Triangle() {

  }

  Triangle(const ZVector<3, T>& p1, const ZVector<3, T>& p2, const ZVector<3, T>& p3) :
    p1(mData[0]),
    p2(mData[1]),
    p3(mData[2])
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

    mData[0] = rhs[0];
    mData[1] = rhs[1];
    mData[2] = rhs[2];
  }

  ZVector<3, T>& operator[](std::size_t index) {
    return mData[index];
  }

  private:
  // TODO: Make this a pointer and update the class to handle that.
  ZVector<3, T> mData[3];
};

}

#endif
