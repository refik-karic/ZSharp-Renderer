#ifndef UTILMATH_H
#define UTILMATH_H

#include <cstdint>
#include "Constants.h"

namespace ZSharp {

/// <summary>
/// Converts degrees to radians.
/// </summary>
/// <param name='degrees'>The angle in degrees.</param>
/// <returns>The angle in radians.</returns>
double DegreesToRadians(const double degrees) {
  return degrees * PI_OVER_180;
}

/// <summary>
/// Calculates an approximate square root for a given value using the Newton-Raphson method.
///
/// Computer Graphics Principles and Practice Second Edition in C
/// James D. Foley, Andries van Dam, Steven K. Feiner, John F. Hughes
/// See Appendix A.7 p.1109
/// </summary>
/// <param name='val'>The value to find the square root of.</param>
/// <returns>An approximate square root.</returns>
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

#endif
