#pragma once

#include <cstddef>
#include <cstring>

#include <array>

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

    mData = rhs.mData;
  }

  T operator[](std::size_t index) const {
    return mData[index];
  }

  T& operator[](std::size_t index) {
    return mData[index];
  }

  void Clear() {
    std::memset(mData.data(), 0, size * sizeof(T));
    mInputIndex = 0;
    mOutputIndex = size / 2;
  }

  void ClipTriangles(VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer) {
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, ClipEdge::POSITIVE_X);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, ClipEdge::POSITIVE_Y);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, ClipEdge::NEGATIVE_X);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, ClipEdge::NEGATIVE_Y);
    SutherlandHodgmanClip(vertexBuffer, indexBuffer, ClipEdge::NEGATIVE_Z);
  }

  private:
  std::array<T, size> mData;
  std::size_t mInputIndex = 0;
  std::size_t mOutputIndex = size / 2;

  enum class ClipEdge {
    POSITIVE_X,
    POSITIVE_Y,
    NEGATIVE_X,
    NEGATIVE_Y,
    NEGATIVE_Z
  };

  void SutherlandHodgmanClip(VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer, ClipEdge clipEdge) {
    std::size_t stride = vertexBuffer.GetStride();
    std::size_t end = indexBuffer.GetWorkingSize();
    for (std::size_t i = 0; i < end; i += Constants::TRI_VERTS, Clear()) {
      const T* v1 = vertexBuffer.GetData() + (indexBuffer[i] * stride);
      const T* v2 = vertexBuffer.GetData() + (indexBuffer[i + 1] * stride);
      const T* v3 = vertexBuffer.GetData() + (indexBuffer[i + 2] * stride);

      ZVector<3, T> start;
      ZVector<3, T> end;
      ZVector<3, T> currentEdge;

      switch (clipEdge) {
        case ClipEdge::POSITIVE_X:
          currentEdge[0] = static_cast<T>(1);

          start.LoadRawData(v1, Constants::TRI_VERTS);
          end.LoadRawData(v2, Constants::TRI_VERTS);

          if (start == end) {
            start[0] += end[0];
          }

          if (v2[0] > currentEdge[0]) {
            T parametricValue = ParametricClipIntersection(start, end, currentEdge, currentEdge);
            ZVector<3, T> clippedEnd = GetParametricVector(parametricValue, start, end);
            // TODO: Come up with a cleaner way to access/store the internal vector data here.
            //AddInputVertex(&clippedEnd[0]);

            if (start == end) {
              start[0] += end[0];
            }
          }
          else {
            AddInputVertex(v2);
          }
          break;
        case ClipEdge::POSITIVE_Y:
          currentEdge[1] = static_cast<T>(1);


          break;
        case ClipEdge::NEGATIVE_X:
          currentEdge[0] = static_cast<T>(-1);


          break;
        case ClipEdge::NEGATIVE_Y:
          currentEdge[1] = static_cast<T>(-1);


          break;
        case ClipEdge::NEGATIVE_Z:
          currentEdge[2] = static_cast<T>(-1);


          break;
      }
    }
  }

  void AddInputVertex(const T* vboData) {
    std::memcpy(mData.data() + mInputIndex, vboData, Constants::TRI_VERTS * sizeof(T));
    mInputIndex += Constants::TRI_VERTS;
  }

  void AddOutputVertex(const T* vboData) {
    std::memcpy(mData.data() + mOutputIndex, vboData, Constants::TRI_VERTS * sizeof(T));
    mOutputIndex += Constants::TRI_VERTS;
  }

  ZVector<3, T> GetParametricVector(T point, ZVector<3, T> start, ZVector<3, T> end) {
    return (start + ((end - start) * point));
  }

  T ParametricClipIntersection(ZVector<3, T> start, ZVector<3, T> end, ZVector<3, T> edgeNormal, ZVector<3, T> edgePoint) {
    T numerator = edgeNormal * (start - edgePoint);
    T denominator = (edgeNormal * static_cast<T>(-1)) * (end - start);

    // TODO: Debug, remove.
    // Was getting a BSOD without this here, which makes me think it was the OS/HW panicking when a divide by 0 was happening.
    if (denominator == static_cast<T>(0)) {
      denominator = static_cast<T>(1);
    }

    return (numerator / denominator);
  }
};

}
