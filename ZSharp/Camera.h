#ifndef CAMERA_H
#define CAMERA_H

#include "ZMatrix.h"
#include "ZVector.h"

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
  Camera(Config config) :
    mConfig(config)
  {
    // Default the look direction is along the negative Z axis.
    mLook[2] = -1;

    // Default the up direction is along the positive Y axis.
    mUp[1] = 1;

    // Default the FOV to be 90 degrees.
    mFovHoriz = 90;
    mFovVert = 90;

    // Default to a near plane of 10 and far plane of 100.
    mNearPlane = 10;
    mFarPlane = 100;
  }

  /// <summary>
  /// Move the camera to a new point in space.
  /// </summary>
  /// <param name='position'>The new location of the camera.</param>
  void MoveCamera(const ZVector<3, T>& position) {
    mPosition = position;
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

  /// <summary>
  /// Camera configuration.
  /// </summary>
  Config mConfig;
};

#endif
