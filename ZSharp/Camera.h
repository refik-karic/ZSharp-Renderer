#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <cstdint>

#include "Constants.h"
#include "IndexBuffer.h"
#include "Triangle.h"
#include "VertexBuffer.h"
#include "ZConfig.h"
#include "ZMatrix.h"
#include "ZVector.h"

namespace ZSharp {

/// <summary>
/// Three dimensional viewing camera.
/// </summary>
/// <typeparam name="T">The type this camera will represent.</typeparam>
template <typename T>
class Camera {
  public:

  /// <summary>
  /// Create a new camera at the origin.
  /// </summary>
  /// <param name='config'>The camera configuration.</param>
  Camera() {
    // Cache some data about the viewport.
    ZConfig& config = ZConfig::GetInstance();
    mWidth = config.GetViewportWidth();
    mHeight = config.GetViewportHeight();

    // Default the look direction is along the negative Z axis.
    mLook[2] = static_cast<T>(-1);

    // Default the up direction is along the positive Y axis.
    mUp[1] = static_cast<T>(1);

    // Default the FOV to be 90 degrees.
    mFovHoriz = static_cast<T>(90);
    mFovVert = static_cast<T>(90);

    // Default to a near plane of 10 and far plane of 100.
    mNearPlane = static_cast<T>(10);
    mFarPlane = static_cast<T>(100);
  }

  /// <summary>
  /// Move the camera to a new point in space.
  /// </summary>
  /// <param name='position'>The new location of the camera.</param>
  void MoveCamera(const ZVector<3, T>& position) {
    mPosition = position;
  }

  void PerspectiveProjection(VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer) {
    // Create perspective projection matrix.

    // Calculate the W unit vector (opposite of look vector).
    ZVector<3, T> w(mLook);
    w = w * static_cast<T>(-1);
    ZVector<3, T>::Normalize(w);

    // Calculate V unit vector (up vector).
    ZVector<3, T> v(mUp);
    v = v - (w * (mUp * w));
    ZVector<3, T>::Normalize(v);

    // Calculate the U unit vector pointing to the right.
    ZVector<3, T> u(v);
    u = ZVector<3, T>::Cross(u, w);

    // At this point U, V, and W have been calculated.
    // Next step is to construct the perspective projection linear transform based off of these unit vectors.

    /*
    * Computer Graphics: Principles and Practice (3rd Edition)
    * John F. Hughes, Andries van Dam, Morgan McGuire, David F. Sklar, James D. Foley, Steven K. Feiner, Kurt Akeley
    *
    * Equation 13.13 p.306 rightmost translation matrix.
    */
    ZMatrix<4, 4, T> translation;
    ZMatrix<4, 4, T>::Identity(translation);
    ZMatrix<4, 4, T>::SetTranslation(translation, (mPosition * static_cast<T>(-1)));

    /*
    * Computer Graphics: Principles and Practice (3rd Edition)
    * John F. Hughes, Andries van Dam, Morgan McGuire, David F. Sklar, James D. Foley, Steven K. Feiner, Kurt Akeley
    *
    * Equation 13.13 p.306 middle "uToE" matrix.
    */
    ZMatrix<4, 4, T> uToE;
    uToE[0] = u;
    uToE[1] = v;
    uToE[2] = w;
    uToE[3][3] = static_cast<T>(1);

    /*
    * Computer Graphics: Principles and Practice (3rd Edition)
    * John F. Hughes, Andries van Dam, Morgan McGuire, David F. Sklar, James D. Foley, Steven K. Feiner, Kurt Akeley
    *
    * Equation 13.13 p.306 leftmost scaling matrix.
    */
    ZMatrix<4, 4, T> scale;
    scale[0][0] = static_cast<T>(1) / (mFarPlane * (std::tan((mFovHoriz * static_cast<T>(Constants::PI_OVER_180) / static_cast<T>(2)))));
    scale[1][1] = static_cast<T>(1) / (mFarPlane * (std::tan((mFovVert * static_cast<T>(Constants::PI_OVER_180) / static_cast<T>(2)))));
    scale[2][2] = static_cast<T>(1) / mFarPlane;
    scale[3][3] = static_cast<T>(1);

    /*
    * "Unhing" transform.
    *
    * Computer Graphics: Principles and Practice (3rd Edition)
    * John F. Hughes, Andries van Dam, Morgan McGuire, David F. Sklar, James D. Foley, Steven K. Feiner, Kurt Akeley
    *
    * See Equation 13.10 on p.308
    */
    ZMatrix<4, 4, T> unhing;
    unhing[0][0] = mFarPlane - mNearPlane;
    unhing[1][1] = mFarPlane - mNearPlane;
    unhing[2][2] = mFarPlane;
    unhing[2][3] = mNearPlane;
    unhing[3][2] = (mFarPlane - mNearPlane) * static_cast<T>(-1);

    // Compound the three transforms to create the standard perspective projection transform (SPPT).
    unhing = unhing * (scale * (uToE * translation));

    /*
    * Create the windowing transform.
    *
    * Computer Graphics: Principles and Practice (3rd Edition)
    * John F. Hughes, Andries van Dam, Morgan McGuire, David F. Sklar, James D. Foley, Steven K. Feiner, Kurt Akeley
    *
    * See Figure 13.1 p.300
    */
    ZMatrix<2, 3, T> windowTransform;
    windowTransform[0][0] = static_cast<T>(static_cast<std::intptr_t>(mWidth) * static_cast<std::intptr_t>(-1));
    windowTransform[0][2] = static_cast<T>(mWidth);
    windowTransform[1][1] = static_cast<T>(mHeight);
    windowTransform[1][2] = static_cast<T>(mHeight);
    windowTransform = windowTransform * (static_cast<T>(1.0 / 2.0));

    // Iterate over each vertex in the VBO.
    for (std::size_t i = 0; i < vertexBuffer.GetWorkingSize(); i += vertexBuffer.GetStride()) {
      // Wrap the vertex data at the current index in a format that can be computed easily.
      ZVector<4, T> vertex;
      vertex.LoadRawData(vertexBuffer.GetData() + i, 3);
      vertex[3] = static_cast<T>(1);

      // Apply the "unhing" transform to each vertex.
      vertex = ZMatrix<4, 4, T>::ApplyTransform(unhing, vertex);

      // Homogenize the verticies.
      ZVector<4, T>::Homogenize(vertex, 3);

      // Drop the W component since it is no longer needed.
      ZVector<4, T>::Homogenize(vertex, 2);

      // Multiply by the windowing transform to get pixel coodinates.
      vertex = ZMatrix<2, 3, T>::ApplyTransform(windowTransform, vertex);

      // Store the resulting vectors back into the vertex table of the mesh.
      vertex.StoreRawData(vertexBuffer.GetData() + i, 2);

      // At this point the current vertex has been converted to screen space and is ready to be drawn.
    }
  }

  private:
  /// <summary>
  /// Contains the cameras position in world space.
  /// </summary>
  ZVector<3, T> mPosition;

  /// <summary>
  /// Look direction vector.
  /// </summary>
  ZVector<3, T> mLook;

  /// <summary>
  /// Up direction vector.
  /// </summary>
  ZVector<3, T> mUp;

  /// <summary>
  /// Near plane scalar.
  /// </summary>
  T mNearPlane;

  /// <summary>
  /// Far plane scalar.
  /// </summary>
  T mFarPlane;

  /// <summary>
  /// Horizontal FOV setting.
  /// </summary>
  T mFovHoriz;

  /// <summary>
  /// Vertical FOV setting.
  /// </summary>
  T mFovVert;

  std::size_t mWidth;
  
  std::size_t mHeight;
};
}

#endif
