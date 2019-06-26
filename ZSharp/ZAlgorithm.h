#pragma once

#include <array>

#include "Constants.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ZMatrix.h"
#include "ZVector.h"

namespace ZSharp {

template <typename T>
class ZAlgorithm final {
  public:
  /// <summary>
  /// Determine whether or not the endpoint is on the inside of the clip edge.
  /// </summary>
  /// <returns>
  /// True if the endpoint is inside the clip edge region.
  /// </returns>
  static bool Inside(const ZVector<3, T>& point, const ZVector<3, T>& clipEdge) {
    /*
    * Check the sign of the dot product of the clip edge with it's difference against and a point in space.
    * Positive means outside, negative means inside, 0 is on the line.
    *
    * Computer Graphics: Principles and Practice Second Edition in C (1995)
    * Foley, van Dam, Feiner, Hughes.
    *
    * Section 3.12.4, A Parametric Line Clipping Algorithm
    * See p.118
    */

    /*
    * Computer Graphics: Principles and Practice (3rd Edition)
    * John F. Hughes, Andries van Dam, Morgan McGuire, David F. Sklar, James D. Foley, Steven K. Feiner, Kurt Akeley
    *
    * See Listing 36.3 on p.1045, Lines 12 and 13. (The listing is in R2 not R3)
    */

    // For some reason that I have yet to understand, both the 2nd and 3rd editions of CGPP define this equation with accepting a different point for subtraction and dot.
    // That is to say, those equations do not share the clip edge in the calculations below like I have.
    // Maybe that is for generalizing the algorithm? At any rate, this does the job for me.
    return ((clipEdge * (point - clipEdge)) <= static_cast<T>(0));
  }

  static ZVector<3, T> GetParametricVector(T point, ZVector<3, T> start, ZVector<3, T> end) {
    return (start + ((end - start) * point));
  }

  static T ParametricLinePlaneIntersection(ZVector<3, T> start, ZVector<3, T> end, ZVector<3, T> edgeNormal, ZVector<3, T> edgePoint) {
    /*
    * Calculates the intersection of a line with a plane in space in R3.
    *
    * Computer Graphics: Principles and Practice Second Edition in C (1995)
    * Foley, van Dam, Feiner, Hughes.
    *
    * Section 3.12.4, A Parametric Line Clipping Algorithm
    * See p.118, Equation 3.1 (simplified)
    */
    T numerator = edgeNormal * (start - edgePoint);
    T denominator = (edgeNormal * static_cast<T>(-1)) * (end - start);
    return (numerator / denominator);
  }

  static std::size_t SutherlandHodgmanClip(std::array<ZVector<3, T>, 6>& inputVerts, std::size_t numInputVerts, const ZVector<3, T>& clipEdge) {
    std::size_t numOutputVerts = 0;
    std::array<ZVector<3, T>, 6> outputVerts;

    for(std::size_t i = 0; i < numInputVerts; ++i) {
      std::size_t nextIndex = (i + 1) % numInputVerts;

      if(!Inside(inputVerts[i], clipEdge) && !Inside(inputVerts[nextIndex], clipEdge)) {
        // Both verticies are outside the clip region, skip.
        continue;
      }
      else if(Inside(inputVerts[i], clipEdge) && Inside(inputVerts[nextIndex], clipEdge)) {
        // Both inside, add unmodified to output.
        outputVerts[numOutputVerts] = inputVerts[nextIndex];
        ++numOutputVerts;
      }
      else {
        const T parametricValue = ParametricLinePlaneIntersection(inputVerts[i], inputVerts[nextIndex], clipEdge, clipEdge);
        const ZVector<3, T> clipPoint = GetParametricVector(parametricValue, inputVerts[i], inputVerts[nextIndex]);

        if(!Inside(inputVerts[i], clipEdge)) {
          // Start point is outside clip region.
          outputVerts[numOutputVerts] = clipPoint;
          ++numOutputVerts;
          outputVerts[numOutputVerts] = inputVerts[nextIndex];
          ++numOutputVerts;
        }
        else {
          // End point is outside clip region.
          outputVerts[numOutputVerts] = clipPoint;
          ++numOutputVerts;
        }
      }
    }

    for(std::size_t i = 0; i < numOutputVerts; ++i) {
      inputVerts[i] = outputVerts[i];
    }

    return numOutputVerts;
  }

  static void CullBackFacingPrimitives(const VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer, ZVector<3, T> viewer) {
    const std::size_t stride = vertexBuffer.GetHomogenizedStride();
    for(std::size_t i = indexBuffer.GetWorkingSize(); i >= Constants::TRI_VERTS; i -= Constants::TRI_VERTS) {
      const T* v1 = vertexBuffer.GetInputData(indexBuffer[i - 3], stride);
      const T* v2 = vertexBuffer.GetInputData(indexBuffer[i - 2], stride);
      const T* v3 = vertexBuffer.GetInputData(indexBuffer[i - 1], stride);
      const ZVector<3, T>& firstEdge = *(reinterpret_cast<const ZVector<3, T>*>(v1));
      const ZVector<3, T>& secondEdge = *(reinterpret_cast<const ZVector<3, T>*>(v2));
      const ZVector<3, T>& thirdEdge = *(reinterpret_cast<const ZVector<3, T>*>(v3));
      ZVector<3, T> triangleNormal = ZVector<3, T>::Cross(firstEdge, thirdEdge);
      ZVector<3, T>::Normalize(triangleNormal);
      T dotResult = (viewer - secondEdge) * triangleNormal;
      if(dotResult <= static_cast<T>(0)) {
        indexBuffer.RemoveTriangle((i / Constants::TRI_VERTS) - 1);
      }
    }
  }

  private:
};

}
