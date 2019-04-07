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
    //Clear();
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
    
    std::size_t stride = vertexBuffer.GetStride();
    std::size_t endEBO = indexBuffer.GetWorkingSize();
    for(std::size_t i = 0; i < endEBO; i += Constants::TRI_VERTS) {
      T* v1 = vertexBuffer.GetData(indexBuffer[i], stride);
      T* v2 = vertexBuffer.GetData(indexBuffer[i + 1], stride);
      T* v3 = vertexBuffer.GetData(indexBuffer[i + 2], stride);
      ZVector<3, T>& v1Vec = *(reinterpret_cast<ZVector<3, T>*>(v1));
      ZVector<3, T>& v2Vec = *(reinterpret_cast<ZVector<3, T>*>(v2));
      ZVector<3, T>& v3Vec = *(reinterpret_cast<ZVector<3, T>*>(v3));

      std::size_t numClippedVerts = 3;
      std::array<ZVector<3, T>, 6> clippedVerts;
      clippedVerts[0] = v1Vec;
      clippedVerts[1] = v2Vec;
      clippedVerts[2] = v3Vec;

      // Positive X (right edge).
      currentEdge[0] = static_cast<T>(1);
      numClippedVerts = SutherlandHodgmanClip(clippedVerts, numClippedVerts, currentEdge);

      // Positive Y (top edge).
      currentEdge[0] = static_cast<T>(0);
      currentEdge[1] = static_cast<T>(1);
      numClippedVerts = SutherlandHodgmanClip(clippedVerts, numClippedVerts, currentEdge);

      // Negative X (left edge).
      currentEdge[0] = static_cast<T>(-1);
      currentEdge[1] = static_cast<T>(0);
      numClippedVerts = SutherlandHodgmanClip(clippedVerts, numClippedVerts, currentEdge);

      // Negative Y (bottom edge).
      currentEdge[0] = static_cast<T>(0);
      currentEdge[1] = static_cast<T>(-1);
      numClippedVerts = SutherlandHodgmanClip(clippedVerts, numClippedVerts, currentEdge);

      // Negative Z (back edge).
      currentEdge[1] = static_cast<T>(0);
      currentEdge[2] = static_cast<T>(-1);
      numClippedVerts = SutherlandHodgmanClip(clippedVerts, numClippedVerts, currentEdge);

      // Add clipped vertices/indices to the output section of the VBO/EBO.
      if(numClippedVerts > 0) {
        std::size_t currentClipIndex = (vertexBuffer.GetWorkingSize() / 4) + (vertexBuffer.GetClipLength() / 3);
        // Verticies are just added to the end in the order they were clipped.
        const T* clippedVertData = reinterpret_cast<const T*>(clippedVerts.data());
        vertexBuffer.Append(clippedVertData, numClippedVerts * Constants::TRI_VERTS);

        // Add the first clipped triangle to the EBO.
        Triangle<T> nextTriangle(currentClipIndex, currentClipIndex + 1, currentClipIndex + 2);
        indexBuffer.Append(nextTriangle);

        // TODO: Hacky solution for now, look for a more general way to solve this since it seems really simple.
        switch(numClippedVerts - Constants::TRI_VERTS) {
          case 1:
            nextTriangle[0] = currentClipIndex + 2;
            nextTriangle[1] = currentClipIndex + 3;
            nextTriangle[2] = currentClipIndex;
            indexBuffer.Append(nextTriangle);
            break;
          case 2:
            nextTriangle[0] = currentClipIndex + 2;
            nextTriangle[1] = currentClipIndex + 3;
            nextTriangle[2] = currentClipIndex + 4;
            indexBuffer.Append(nextTriangle);
            // TODO: It might make more sense to order these 0, 2, 4 here?
            nextTriangle[0] = currentClipIndex + 4;
            nextTriangle[1] = currentClipIndex;
            nextTriangle[2] = currentClipIndex + 0;
            indexBuffer.Append(nextTriangle);
            break;
          case 3:
            nextTriangle[0] = currentClipIndex + 2;
            nextTriangle[1] = currentClipIndex + 3;
            nextTriangle[2] = currentClipIndex + 4;
            indexBuffer.Append(nextTriangle);
            nextTriangle[0] = currentClipIndex + 4;
            nextTriangle[1] = currentClipIndex + 5;
            nextTriangle[2] = currentClipIndex;
            indexBuffer.Append(nextTriangle);
            nextTriangle[0] = currentClipIndex;
            nextTriangle[1] = currentClipIndex + 2;
            nextTriangle[2] = currentClipIndex + 4;
            indexBuffer.Append(nextTriangle);
            break;
        }
      }
    }
#if 0
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
#endif
  }

  private:
  T mData[size];
  std::size_t mInputIndex = 0;
  std::size_t mOutputIndex = size / 2;

  std::size_t SutherlandHodgmanClip(std::array<ZVector<3, T>, 6>& inputVerts, std::size_t numInputVerts, ZVector<3, T>& clipEdge) {
    std::size_t numOutputVerts = 0;
    std::array<ZVector<3, T>, 6> outputVerts;

    for(std::size_t i = 0; i < numInputVerts; ++i) {
      std::size_t nextIndex = (i + 1) % numInputVerts;

      if(!Inside(inputVerts[i], clipEdge) && !Inside(inputVerts[nextIndex], clipEdge)){
        // Both verticies are outside the clip region, skip.
        continue;
      }
      else if(Inside(inputVerts[i], clipEdge) && Inside(inputVerts[nextIndex], clipEdge)) {
        // Both inside, add unmodified to output.
        outputVerts[numOutputVerts] = inputVerts[i];
        ++numOutputVerts;
      }
      else {
        T parametricValue = ParametricClipIntersection(inputVerts[i], inputVerts[nextIndex], clipEdge, clipEdge);
        ZVector<3, T> clipPoint = GetParametricVector(parametricValue, inputVerts[i], inputVerts[nextIndex]);

        if(!Inside(inputVerts[nextIndex], clipEdge)){
          // End point is outside clip region, add to output section of clip buffer.
          outputVerts[numOutputVerts] = inputVerts[i];
          ++numOutputVerts;
          outputVerts[numOutputVerts] = clipPoint;
          ++numOutputVerts;
        }
        else if(!Inside(inputVerts[i], clipEdge)){
          // Start point is outside clip region, add to input section of clip buffer.
          outputVerts[numOutputVerts] = clipPoint;
          ++numOutputVerts;
          outputVerts[numOutputVerts] = inputVerts[i];
          ++numOutputVerts;
        }
      }
    }

    inputVerts = outputVerts;
    return numOutputVerts;
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
  bool Inside(ZVector<3, T>& point, ZVector<3, T>& clipEdge) {
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
