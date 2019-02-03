#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <cstdint>

#include <array>
#include <map>

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

    // Setup the planes for clipping.
    BuildClipPlanes();
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

    std::size_t stride = vertexBuffer.GetStride();
    std::size_t end = indexBuffer.GetWorkingSize();
    for (std::size_t i = 0; i < end; i += TRI_VERTS) {
      // Get the indicies into the VBO from the EBO.
      T* v1 = vertexBuffer.GetData() + (indexBuffer[i] * stride);
      T* v2 = vertexBuffer.GetData() + (indexBuffer[i + 1] * stride);
      T* v3 = vertexBuffer.GetData() + (indexBuffer[i + 2] * stride);

      // Store the data in a format we can compute.
      ZVector<4, T> p1;
      p1.LoadRawData(v1, TRI_VERTS);
      p1[3] = static_cast<T>(1);

      ZVector<4, T> p2;
      p2.LoadRawData(v2, TRI_VERTS);
      p2[3] = static_cast<T>(1);

      ZVector<4, T> p3;
      p3.LoadRawData(v3, TRI_VERTS);
      p3[3] = static_cast<T>(1);

      // Apply the "unhing" transform to each vertex in the primitive.
      p1 = ZMatrix<4, 4, T>::ApplyTransform(unhing, p1);
      p2 = ZMatrix<4, 4, T>::ApplyTransform(unhing, p2);
      p3 = ZMatrix<4, 4, T>::ApplyTransform(unhing, p3);

      // TODO: Clip in R4 to remove points with Z < 0.
      // This step can be skipped for now since my primitives should always be in front of the camera.

      // Homogenize the verticies to bring them into SPVV space.
      ZVector<4, T>::Homogenize(p1, 3);
      ZVector<4, T>::Homogenize(p2, 3);
      ZVector<4, T>::Homogenize(p3, 3);

      p1.StoreRawData(v1, 3);
      p2.StoreRawData(v2, 3);
      p3.StoreRawData(v3, 3);
    }

    // At this point the vertex is transformed into the "SPVV".
    // TODO: Clip to the SPVV view volume on all sides but the near plane.
    // NOTE: Only need to clip to +-1x, +-1y, and -1z. Near clipping should be done before homogenizing the W factor.
    // At this point the current primitive has been converted to screen space and is ready to be drawn.
    // TODO: Call SutherlandHodgmanClip()

    // Get the update sizes of each buffer after clipping.
    stride = vertexBuffer.GetStride();
    end = indexBuffer.GetWorkingSize();
    for (std::size_t i = 0; i < end; i += TRI_VERTS) {
      T* v1 = vertexBuffer.GetData() + (indexBuffer[i] * stride);
      T* v2 = vertexBuffer.GetData() + (indexBuffer[i + 1] * stride);
      T* v3 = vertexBuffer.GetData() + (indexBuffer[i + 2] * stride);

      // Store the data in a format we can compute.
      ZVector<3, T> p1;
      p1.LoadRawData(v1, TRI_VERTS);

      ZVector<3, T> p2;
      p2.LoadRawData(v2, TRI_VERTS);

      ZVector<3, T> p3;
      p3.LoadRawData(v3, TRI_VERTS);

      // Drop the W component since it is no longer needed.
      ZVector<3, T>::Homogenize(p1, 2);
      ZVector<3, T>::Homogenize(p2, 2);
      ZVector<3, T>::Homogenize(p3, 2);

      // Multiply by the windowing transform to get pixel coodinates.
      p1 = ZMatrix<2, 3, T>::ApplyTransform(windowTransform, p1);
      p2 = ZMatrix<2, 3, T>::ApplyTransform(windowTransform, p2);
      p3 = ZMatrix<2, 3, T>::ApplyTransform(windowTransform, p3);

      // Store the resulting vectors back into the VBO.
      p1.StoreRawData(v1, 2);
      p2.StoreRawData(v2, 2);
      p3.StoreRawData(v3, 2);
    }

    // All verticies are in "SPVV" space (i.e. view/camera space), clip edges where necessary.
    // We are using a RHCS, meaning that all verticies are defined in CLOCKWISE order!
    // TODO: Use the clockwise ordering of the NDC cubic-volume to calculate the Normal "orientation vectors"
    // Then use these orientation vectors to perform Sutherland-Hodgeman clipping on all necessary planes of the cubic volume.
    //    **Still don't know if clipping to all 6 planes will be necessary, look into this futher.**

    // Send all remaining verticies to screen space.
    // TODO: Write the loop over the clipped edges here that sends them back to screen space.
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

  enum class ClipPlane {
    BACK,
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
  };

  typedef std::array<ZVector<3, T>, 2> ClipEdge_t;

  typedef std::array<ClipEdge_t, 4> PlaneEdges_t;

  std::map<ClipPlane, PlaneEdges_t> mClipPlanes;

  void BuildClipPlanes() {
    // NOTE: All planes are defined in counter-clockwise order so that the normal points away from the inside of each clip plane.
    ZVector<3, T> p1;
    ZVector<3, T> p2;
    ClipEdge_t edge;
    PlaneEdges_t plane;

    // BACK
    {
      p1[0] = static_cast<T>(1);
      p1[1] = static_cast<T>(1);
      p1[2] = static_cast<T>(-1);
      p2[0] = static_cast<T>(-1);
      p2[1] = static_cast<T>(1);
      p2[2] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[0] = edge;

      p1 = p2;
      p2[1] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[1] = edge;

      p1 = p2;
      p2[0] = static_cast<T>(1);
      edge[0] = p1;
      edge[1] = p2;
      plane[2] = edge;

      p1 = p2;
      p2[1] = static_cast<T>(1);
      edge[0] = p1;
      edge[1] = p2;
      plane[3] = edge;

      mClipPlanes.insert(std::pair<ClipPlane, PlaneEdges_t>(ClipPlane::BACK, plane));
    }

    // LEFT
    {
      p1[0] = static_cast<T>(-1);
      p1[1] = static_cast<T>(1);
      p1[2] = static_cast<T>(-1);
      p2[0] = static_cast<T>(-1);
      p2[1] = static_cast<T>(1);
      p2[2] = static_cast<T>(0);
      edge[0] = p1;
      edge[1] = p2;
      plane[0] = edge;

      p1 = p2;
      p2[1] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[1] = edge;

      p1 = p2;
      p2[2] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[2] = edge;

      p1 = p2;
      p2[1] = static_cast<T>(1);
      edge[0] = p1;
      edge[1] = p2;
      plane[3] = edge;

      mClipPlanes.insert(std::pair<ClipPlane, PlaneEdges_t>(ClipPlane::LEFT, plane));
    }

    // RIGHT
    {
      p1[0] = static_cast<T>(1);
      p1[1] = static_cast<T>(1);
      p1[2] = static_cast<T>(0);
      p2[0] = static_cast<T>(1);
      p2[1] = static_cast<T>(1);
      p2[2] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[0] = edge;

      p1 = p2;
      p2[1] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[1] = edge;

      p1 = p2;
      p2[0] = static_cast<T>(0);
      edge[0] = p1;
      edge[1] = p2;
      plane[2] = edge;

      p1 = p2;
      p2[1] = static_cast<T>(1);
      edge[0] = p1;
      edge[1] = p2;
      plane[3] = edge;

      mClipPlanes.insert(std::pair<ClipPlane, PlaneEdges_t>(ClipPlane::RIGHT, plane));
    }

    // BOTTOM
    {
      p1[0] = static_cast<T>(1);
      p1[1] = static_cast<T>(-1);
      p1[2] = static_cast<T>(-1);
      p2[0] = static_cast<T>(-1);
      p2[1] = static_cast<T>(-1);
      p2[2] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[0] = edge;

      p1 = p2;
      p2[2] = static_cast<T>(0);
      edge[0] = p1;
      edge[1] = p2;
      plane[1] = edge;

      p1 = p2;
      p2[0] = static_cast<T>(1);
      edge[0] = p1;
      edge[1] = p2;
      plane[2] = edge;

      p1 = p2;
      p2[2] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[3] = edge;

      mClipPlanes.insert(std::pair<ClipPlane, PlaneEdges_t>(ClipPlane::BOTTOM, plane));
    }

    // TOP
    {
      p1[0] = static_cast<T>(1);
      p1[1] = static_cast<T>(1);
      p1[2] = static_cast<T>(0);
      p2[0] = static_cast<T>(-1);
      p2[1] = static_cast<T>(1);
      p2[2] = static_cast<T>(0);
      edge[0] = p1;
      edge[1] = p2;
      plane[0] = edge;

      p1 = p2;
      p2[2] = static_cast<T>(-1);
      edge[0] = p1;
      edge[1] = p2;
      plane[1] = edge;

      p1 = p2;
      p2[0] = static_cast<T>(1);
      edge[0] = p1;
      edge[1] = p2;
      plane[2] = edge;

      p1 = p2;
      p2[2] = static_cast<T>(0);
      edge[0] = p1;
      edge[1] = p2;
      plane[3] = edge;

      mClipPlanes.insert(std::pair<ClipPlane, PlaneEdges_t>(ClipPlane::TOP, plane));
    }
  }

  void SutherlandHodgman3dClip(VertexBuffer<T>& vertexBuffer, IndexBuffer& indexBuffer, PlaneEdges_t planeEdges) {
    // TODO: Implement this.
  }
};
}

#endif
