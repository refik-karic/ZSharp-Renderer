#include <algorithm>
#include <cmath>

#include "ZDrawing.h"

namespace ZSharp {

void ZDrawing::DrawRunSlice(Framebuffer& framebuffer, 
                  std::int32_t x1, 
                  std::int32_t y1, 
                  std::int32_t x2, 
                  std::int32_t y2, 
                  ZColor color) {
  if (x1 == x2) {
    // Special case for vertical lines.
    if (y1 == y2) {
      // Zero length line, stop drawing.
      return;
    }

    // Swap values if needed.
    if (y2 < y1) {
      std::int32_t temp = y2;
      y2 = y1;
      y1 = temp;
    }

    // Since Y access goes by column, cache efficiency can be ignored.
    while (y1 < y2) {
      framebuffer.SetPixel(x1, y1, color);
      y1++;
    }
  }
  else if (y1 == y2) {
    // Special case for horizontal lines.
    if (x1 == x2) {
      // Zero length line, stop drawing.
      return;
    }

    // Swap to always go left to right and maintain efficient use of HW cache.
    if (x2 < x1) {
      std::int32_t temp = x2;
      x2 = x1;
      x1 = temp;
    }

    // NOTE: Since this is moving left to right it can be replaced by a memset which will be much faster.
    // This is because it is very likely that the library memset will use an instrinsic version specific to the target instruction set.
    while (x1 < x2) {
      framebuffer.SetPixel(x1, y1, color);
      x1++;
    }
  }
  else {
    // Note that slope is always positive in all calculations.
    // Slope is also flipped depending on the major axis.
    double slope;
    // Error is always treated as positive.
    double error = 0.0;
    // Keeps track of how many pixels the current row will draw.
    std::int32_t slopeStep;
    // Total number of iterations alone the minor axis.
    std::int32_t delta;

    // Always make sure to draw upwards by swapping points such that P1y is always less than P2y.
    if (y2 < y1) {
      std::int32_t temp = y2;
      y2 = y1;
      y1 = temp;

      temp = x2;
      x2 = x1;
      x1 = temp;
    }

    // Diagonal lines.
    // TODO: It may be worth special casing diagonals with a slope of 1 since the error term becomes meaningless.
    if (std::abs(x2 - x1) >= std::abs(y2 - y1)) {
      // Y minor axis.
      delta = std::abs(y2 - y1);
      slope = std::abs(static_cast<double>((x2 - x1)) / (y2 - y1));

      for (std::size_t i = 0; i < delta; i++) {
        // Drop fraction.
        error = error + (slope - std::floor(slope));
        slopeStep = static_cast<std::int32_t>(std::floor(slope) + error);

        // Compensate for the extra pixel.
        if (slopeStep > static_cast<std::int32_t>(slope)) {
          error = std::max(error - 1.0, 0.0);
        }

        if (x2 <= x1) {
          // Moving in the negative X direction but still drawing left to right.
          for (std::size_t j = x1 - slopeStep; j < x1; j++) {
            framebuffer.SetPixel(j, y1, color);
          }

          x1 -= slopeStep;
        }
        else {
          // Draw a horizontal run slice left to right.
          for (std::size_t j = x1; j < x1 + slopeStep; j++) {
            framebuffer.SetPixel(j, y1, color);
          }

          x1 += slopeStep;
        }

        y1++; // Y values are swapped so we always move up along the minor axis.
      }
    }
    else {
      // X minor axis.
      delta = std::abs(x2 - x1);
      // Keeps track of how much, but more importantly which direction, to move along the minor axis.
      std::int32_t minorStep = (x2 - x1) / delta;
      slope = std::abs(static_cast<double>((y2 - y1)) / (x2 - x1));

      for (std::size_t i = 0; i < delta; i++) {
        // Drop fraction.
        error = error + (slope - std::floor(slope));
        slopeStep = static_cast<std::int32_t>(std::floor(slope) + error);

        // Compensate for the extra pixel.
        if (slopeStep > static_cast<std::int32_t>(slope)) {
          error = std::max(error - 1.0, 0.0);
        }

        // Draw a vertical run slice.
        for (std::size_t j = y1; j < y1 + slopeStep; j++) {
          framebuffer.SetPixel(x1, j, color);
        }

        // Must adjust the x by this even since it only moves one pixel at a time.
        // This is because it could be moving left or right.
        x1 += minorStep;
        y1 += slopeStep;
      }
    }
  }
}

}