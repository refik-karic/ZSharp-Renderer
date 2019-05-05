#pragma once

#include <cstddef>

namespace ZSharp {

/// <summary>
/// Collection of useful compile time constant values.
/// </summary>
class Constants {
  public:
  /// <summary>
  /// Don't allow this class to be instantiated.
  /// </summary>
  Constants() = delete;

  static constexpr float FRAMERATE_60HZ_MS = 1000.0F / 60.0F;

  static constexpr double PI = 3.14159265358979;

  static constexpr double PI_OVER_180 = PI / 180.0;

  static constexpr std::size_t MAX_VERTS_AFTER_CLIP = 2;
  static constexpr std::size_t MAX_INDICIES_AFTER_CLIP = 4;

  // This is obvious and seemingly makes the code more verbose than necessary.
  // It's just meant to be an aid for finding instances in the code where something might need to know this.
  static constexpr std::size_t TRI_VERTS = 3;

  static constexpr std::size_t R3 = 3;
};

}
