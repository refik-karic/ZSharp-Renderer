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
  static bool Inside(const ZVector<3, T>& point, const ZVector<3, T>& clipEdge) {
    return ((clipEdge * (point - clipEdge)) <= static_cast<T>(0));
  }

  static ZVector<3, T> GetParametricVector(T point, ZVector<3, T> start, ZVector<3, T> end) {
    return (start + ((end - start) * point));
  }

  static T ParametricLinePlaneIntersection(ZVector<3, T> start, ZVector<3, T> end, ZVector<3, T> edgeNormal, ZVector<3, T> edgePoint) {
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
        continue;
      }
      else if(Inside(inputVerts[i], clipEdge) && Inside(inputVerts[nextIndex], clipEdge)) {
        outputVerts[numOutputVerts] = inputVerts[nextIndex];
        ++numOutputVerts;
      }
      else {
        const T parametricValue = ParametricLinePlaneIntersection(inputVerts[i], inputVerts[nextIndex], clipEdge, clipEdge);
        const ZVector<3, T> clipPoint = GetParametricVector(parametricValue, inputVerts[i], inputVerts[nextIndex]);

        if(!Inside(inputVerts[i], clipEdge)) {
          outputVerts[numOutputVerts] = clipPoint;
          ++numOutputVerts;
          outputVerts[numOutputVerts] = inputVerts[nextIndex];
          ++numOutputVerts;
        }
        else {
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
      ZVector<3, T> p1p0 = secondEdge - firstEdge;
      ZVector<3, T> p2p1 = thirdEdge - secondEdge;
      ZVector<3, T> triangleNormal = ZVector<3, T>::Cross(p1p0, p2p1);
      T dotResult = (viewer - secondEdge) * triangleNormal;
      if(dotResult <= static_cast<T>(0)) {
        indexBuffer.RemoveTriangle((i / Constants::TRI_VERTS) - 1);
      }
    }
  }

  private:
};

}
