#ifndef CONSTANTS_H
#define CONSTANTS_H


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
};

}

#endif
