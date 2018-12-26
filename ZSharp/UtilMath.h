#ifndef UTILMATH_H
#define UTILMATH_H

#include <cstdint>
#include "Constants.h"

// Warning	C4723	potential divide by 0
#pragma warning(disable: 4723)

namespace ZSharp {

/// <summary>
/// Converts degrees to radians.
/// </summary>
/// <param name='degrees'>The angle in degrees.</param>
/// <returns>The angle in radians.</returns>
double DegreesToRadians(const double degrees);

template <typename T>
T NewtonRaphsonSqrt(T val) {
  // Adjust as necessary for accuracy.
  uint32_t iterations = 10;

  // Initial guess.
  T x0 = {};

  for (uint32_t i = 0; i < iterations; i++) {
    /*
    x0 is the initial guess that gets adjusted after each iteration.
    Basically this boils down to the following mathematical equation:
    for N => guess = guess - f(guess^2 - argument) / f'(guess)
    Numerator is a function f(x), denominator is its derivative f'(x).
    i.e. f(x) = x^2 - y, f'(x) = 2x
    */
    x0 = x0 - (((x0 * x0) - val) / (2 * x0));
  }

  return x0;
}
}

// Warning	C4723	potential divide by 0
#pragma warning(default: 4723)

#endif
