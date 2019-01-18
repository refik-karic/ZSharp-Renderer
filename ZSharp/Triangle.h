#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cstddef>

// This is obvious and seemingly makes the code more verbose than necessary.
// It's just meant to be an aid for finding instances in the code where something might need to know this.
static constexpr std::size_t TRI_VERTS = 3;

namespace ZSharp {

template<typename T>
class Triangle {
  public:

  Triangle() {}

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

    mIndicies[0] = rhs[0];
    mIndicies[1] = rhs[1];
    mIndicies[2] = rhs[2];
  }

  std::size_t operator[](std::size_t index) const {
    return mIndicies[index];
  }

  std::size_t& operator[](std::size_t index) {
    return mIndicies[index];
  }

  const std::size_t* GetData() {
    return mIndicies;
  }

  private:
  std::size_t mIndicies[3] = {0, 0, 0};
};

}

#endif
