#pragma once

#include <cstdint>
#include <cstddef>

#include "Constants.h"
#include "Framebuffer.h"
#include "IndexBuffer.h"
#include "Triangle.h"
#include "VertexBuffer.h"
#include "ZColor.h"

namespace ZSharp {

void DrawRunSlice(Framebuffer& framebuffer, 
                  std::int32_t x1, 
                  std::int32_t y1, 
                  std::int32_t x2, 
                  std::int32_t y2, 
                  ZColor color);

template<typename T>
void DrawTriangles(Framebuffer& framebuffer, 
                   const VertexBuffer<T>& vertexBuffer, 
                   const IndexBuffer& indexBuffer, 
                   ZColor color) {
  // Iterate over each triangle using the EBO.
  // This means the VBO can have gaps in its data as a result of clipping but those will be skipped
  //    if the indexing here is done purely using the EBO!
  //    (i.e.) indicies can be easily swapped elsewhere without having to rearrange the VBO on each clip operation.
  std::size_t stride = vertexBuffer.GetStride();
  std::size_t end = indexBuffer.GetWorkingSize();
  for (std::size_t i = 0; i < end; i += Constants::TRI_VERTS) {
    const T* v1 = vertexBuffer.GetData() + (indexBuffer[i] * stride);
    const T* v2 = vertexBuffer.GetData() + (indexBuffer[i + 1] * stride);
    const T* v3 = vertexBuffer.GetData() + (indexBuffer[i + 2] * stride);

    // Draw line connecting v1 to v2.
    DrawRunSlice(framebuffer,
                 static_cast<std::size_t>(*(v1)),
                 static_cast<std::size_t>(*(v1 + 1)),
                 static_cast<std::size_t>(*(v2)),
                 static_cast<std::size_t>(*(v2 + 1)),
                 color);

    // Draw line connecting v2 to v3.
    DrawRunSlice(framebuffer,
                 static_cast<std::size_t>(*(v2)),
                 static_cast<std::size_t>(*(v2 + 1)),
                 static_cast<std::size_t>(*(v3)),
                 static_cast<std::size_t>(*(v3 + 1)),
                 color);

    // Draw line connecting v3 to v1.
    DrawRunSlice(framebuffer,
                 static_cast<std::size_t>(*(v3)),
                 static_cast<std::size_t>(*(v3 + 1)),
                 static_cast<std::size_t>(*(v1)),
                 static_cast<std::size_t>(*(v1 + 1)),
                 color);
  }
}

}
