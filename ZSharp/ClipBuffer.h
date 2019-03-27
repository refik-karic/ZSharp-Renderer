#pragma once

#include <cstddef>
#include <cstring>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ZVector.h"

namespace ZSharp {

template<typename T, std::size_t size>
class ClipBuffer {
  public:
  ClipBuffer() {
    Clear();
  }

  ClipBuffer(const ClipBuffer<T, size>& rhs) {
    if (this != &rhs) {
      *this = rhs;
    }
  }

  void operator=(const ClipBuffer<T, size>& rhs) {
    if (this == &rhs) {
      return;
    }

    std::memcpy(mData, rhs.mData, size * sizeof(T));
  }

  T operator[](std::size_t index) const {
    return mData[index];
  }

  T& operator[](std::size_t index) {
    return mData[index];
  }

  void Clear() {
    // TODO: Figure out why this is causing a BSOD.
    std::memset(mData, 0, size * sizeof(T));
    mInputIndex = 0;
    mOutputIndex = size / 2;
  }

  void ClipTriangles(VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer) {
    ZVector<3, T> currentEdge;
    
    // Positive X (right edge).
    currentEdge[0] = static_cast<T>(1);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, currentEdge);

    // Positive Y (top edge).
    currentEdge[0] = static_cast<T>(0);
    currentEdge[1] = static_cast<T>(1);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, currentEdge);
    
    // Negative X (left edge).
    currentEdge[0] = static_cast<T>(-1);
    currentEdge[1] = static_cast<T>(0);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, currentEdge);
    
    // Negative Y (bottom edge).
    currentEdge[0] = static_cast<T>(0);
    currentEdge[1] = static_cast<T>(-1);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, currentEdge);
    
    // Negative Z (back edge).
    currentEdge[1] = static_cast<T>(0);
    currentEdge[2] = static_cast<T>(-1);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, currentEdge);
  }

  private:
  T mData[size];
  std::size_t mInputIndex = 0;
  std::size_t mOutputIndex = size / 2;

  void SutherlandHodgmanClip(VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer, ZVector<3, T> clipEdge) {
    std::size_t stride = vertexBuffer.GetStride();
    std::size_t endEBO = indexBuffer.GetWorkingSize();
    for (std::size_t i = 0; i < endEBO; i += Constants::TRI_VERTS) {
      const T* v1 = vertexBuffer.GetData() + (indexBuffer[i] * stride);
      const T* v2 = vertexBuffer.GetData() + (indexBuffer[i + 1] * stride);
      const T* v3 = vertexBuffer.GetData() + (indexBuffer[i + 2] * stride);

      ZVector<3, T> startPoint;
      ZVector<3, T> endPoint;

      startPoint.LoadRawData(v1, Constants::TRI_VERTS);
      endPoint.LoadRawData(v2, Constants::TRI_VERTS);

      // TODO: Check for zero length lines to avoid a divide by 0!

      if (!Inside(startPoint, endPoint, clipEdge) && !Inside(endPoint, startPoint, clipEdge)) {
        // Both verticies are outside the clip region, skip.
        continue;
        // TODO: Need to update the VBO/EBO here when verticies are skipped.

      }
      else if (!Inside(startPoint, endPoint, clipEdge)) {
        // End point is outside clip region, add to output section of clip buffer.
        T parametricValue = ParametricClipIntersection(startPoint, endPoint, clipEdge, clipEdge);
        ZVector<3, T> clippedEnd = GetParametricVector(parametricValue, startPoint, endPoint);

      }
      else if(!Inside(endPoint, startPoint, clipEdge)) {
        // Start point is outside clip region, add to input section of clip buffer.
        //T parametricValue = ParametricClipIntersection(end, start, clipEdge, clipEdge);
        //ZVector<3, T> clippedEnd = GetParametricVector(parametricValue, end, start);
        T parametricValue = ParametricClipIntersection(startPoint, endPoint, clipEdge, clipEdge);
        ZVector<3, T> clippedEnd = GetParametricVector(parametricValue, startPoint, endPoint);

      }
      else {
        // Add existing vertex to input section of the clip buffer.

      }

      // Second edge.
      startPoint.LoadRawData(v2, Constants::TRI_VERTS);
      endPoint.LoadRawData(v3, Constants::TRI_VERTS);

      if (!Inside(startPoint, endPoint, clipEdge) && !Inside(endPoint, startPoint, clipEdge)) {
        // Both verticies are outside the clip region, skip.
        continue;
        // TODO: Need to update the VBO/EBO here when verticies are skipped.
      } else if (!Inside(startPoint, endPoint, clipEdge)) {
        // End point is outside clip region, add to output section of clip buffer.
        //T parametricValue = ParametricClipIntersection(start, end, clipEdge, clipEdge);
        //ZVector<3, T> clippedEnd = GetParametricVector(parametricValue, start, end);

      } else if (!Inside(endPoint, startPoint, clipEdge)) {
        // Start point is outside clip region, add to input section of clip buffer.
        //T parametricValue = ParametricClipIntersection(end, start, clipEdge, clipEdge);
        //ZVector<3, T> clippedEnd = GetParametricVector(parametricValue, end, start);

      } else {
        // Add existing vertex to input section of the clip buffer.

      }
      
      // Third and final edge.
      startPoint.LoadRawData(v3, Constants::TRI_VERTS);
      endPoint.LoadRawData(v1, Constants::TRI_VERTS);

      if (!Inside(startPoint, endPoint, clipEdge) && !Inside(endPoint, startPoint, clipEdge)) {
        // Both verticies are outside the clip region, skip.
        continue;
        // TODO: Need to update the VBO/EBO here when verticies are skipped.
      } else if (!Inside(startPoint, endPoint, clipEdge)) {
        // End point is outside clip region, add to output section of clip buffer.
        //T parametricValue = ParametricClipIntersection(start, end, clipEdge, clipEdge);
        //ZVector<3, T> clippedEnd = GetParametricVector(parametricValue, start, end);

      } else if (!Inside(endPoint, startPoint, clipEdge)) {
        // Start point is outside clip region, add to input section of clip buffer.
        //T parametricValue = ParametricClipIntersection(end, start, clipEdge, clipEdge);
        //ZVector<3, T> clippedEnd = GetParametricVector(parametricValue, end, start);

      } else {
        // Add existing vertex to input section of the clip buffer.

      }

      //Clear();
    }
  }

  void AddInputVertex(const T* vboData) {
    std::memcpy(mData + mInputIndex, vboData, Constants::TRI_VERTS * sizeof(T));
    mInputIndex += Constants::TRI_VERTS;
  }

  void AddOutputVertex(const T* vboData) {
    std::memcpy(mData + mOutputIndex, vboData, Constants::TRI_VERTS * sizeof(T));
    mOutputIndex += Constants::TRI_VERTS;
  }

  /// <summary>
  /// Determine whether or not the endpoint is on the inside of the clip edge.
  /// </summary>
  /// <returns>
  /// True if the endpoint is inside the clip edge region.
  /// </returns>
  bool Inside(ZVector<3, T>& start, ZVector<3, T>& end, ZVector<3, T>& clipEdge) {
    /*
    * Check the sign of the dot product of the clip edge and the difference vector of the line.
    * Positive means outside, negative means inside, 0 is on the line.
    *
    * Computer Graphics: Principles and Practice Second Edition in C (1995)
    * Foley, van Dam, Feiner, Hughes.
    * 
    * Section 3.12.4, A Parametric Line Clipping Algorithm
    * See p.118
    */
    (void)start;
    (void)end;
    (void)clipEdge;

    return true;
    //return ((clipEdge * (end - start)) < static_cast<T>(0));
  }

  ZVector<3, T> GetParametricVector(T point, ZVector<3, T> start, ZVector<3, T> end) {
    return (start + ((end - start) * point));
  }

  T ParametricClipIntersection(ZVector<3, T> start, ZVector<3, T> end, ZVector<3, T> edgeNormal, ZVector<3, T> edgePoint) {
    T numerator = edgeNormal * (start - edgePoint);
    T denominator = (edgeNormal * static_cast<T>(-1)) * (end - start);
    return (numerator / denominator);
  }
};

}
