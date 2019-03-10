#pragma once

#include <cmath>
#include <cstdint>

#include <array>
#include <map>

#include "ClipBuffer.h"
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

  ZVector<3, T> GetPosition() const {
    return mPosition;
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

    /*
    * Computer Graphics: Principles and Practice (3rd Edition)
    * John F. Hughes, Andries van Dam, Morgan McGuire, David F. Sklar, James D. Foley, Steven K. Feiner, Kurt Akeley
    *
    * Equations [13.2, 13.5] p.304
    * Note that this uses the approach described in Inline Exercise 13.1 which doesn't negate the W vector.
    * Part of the reasoning for this is performance, since that approach requires an extra scalar multiply to negate the look vector.
    * The actual reason is because the camera was pointing in the opposite direction that I was expecting.
    * Ignoring the negation fixed the issue, which I think may have been because the text was describing a LHCS camera view (Figure 13.6 is quite confusing).
    */

    // Calculate the W unit vector (opposite of look vector).
    ZVector<3, T> w;
    w = mLook;
    ZVector<3, T>::Normalize(w);

    // Calculate V unit vector (up vector).
    ZVector<3, T> v;
    v = mUp - (w * (mUp * w));
    ZVector<3, T>::Normalize(v);

    // Calculate the U unit vector pointing to the right.
    ZVector<3, T> u;
    u = ZVector<3, T>::Cross(w, v);

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

    // Compound the three transforms to create the standard parallel view volume (SPVV).
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
    windowTransform[0][0] = static_cast<T>(mWidth);
    windowTransform[0][2] = static_cast<T>(mWidth);
    windowTransform[1][1] = static_cast<T>((mHeight * static_cast<T>(-1)));
    windowTransform[1][2] = static_cast<T>(mHeight);
    windowTransform = windowTransform * (static_cast<T>(1.0 / 2.0));

    // TODO: This implementation will currently apply the transformaton to ALL verticies in the VBO, regardless if they are vertex data in a stride or texture coordinates!
    // TODO: Keep in mind clipped verticies when performing operations over all verticies in the VBO!
    // TODO: Also, be wary of shared verticies when implementing clipping to preserve memory for shared input verticies!
    
    for (std::size_t i = 0; i < vertexBuffer.GetWorkingSize(); i += 4) {
      T* vertexData = vertexBuffer.GetData() + i;
      ZVector<4, T> vertexVector;
      vertexVector.LoadRawData(vertexData, Constants::TRI_VERTS);
      vertexVector[3] = static_cast<T>(1);
      vertexVector = ZMatrix<4, 4, T>::ApplyTransform(unhing, vertexVector);
      vertexVector.StoreRawData(vertexData, 4);
    }

    // TODO: Clip in R4 to remove points with Z < 0.
    // This step can be skipped for now since my primitives should always be in front of the camera.

    for (std::size_t i = 0; i < vertexBuffer.GetWorkingSize(); i += 4) {
      T* vertexData = vertexBuffer.GetData() + i;
      ZVector<4, T> vertexVector;
      vertexVector.LoadRawData(vertexData, 4);
      ZVector<4, T>::Homogenize(vertexVector, 3);
      vertexVector.StoreRawData(vertexData, 3);
    }

    // At this point all verticies have been transformed into the "SPVV".
    mClipBuffer.ClipTriangles(vertexBuffer, indexBuffer);

    for (std::size_t i = 0; i < vertexBuffer.GetWorkingSize(); i += 4) {
      T* vertexData = vertexBuffer.GetData() + i;
      ZVector<4, T> vertexVector;
      vertexVector.LoadRawData(vertexData, Constants::TRI_VERTS);
      ZVector<4, T>::Homogenize(vertexVector, 2);
      vertexVector = ZMatrix<2, 3, T>::ApplyTransform(windowTransform, vertexVector);
      vertexVector.StoreRawData(vertexData, 2);
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

  std::intptr_t mWidth;
  
  std::intptr_t mHeight;

  ClipBuffer<T, Constants::TRI_CLIP_BUFFER_SIZE> mClipBuffer;
};
}
