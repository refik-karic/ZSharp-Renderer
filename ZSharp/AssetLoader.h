#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <locale>
#include <string>

#include "Model.h"

namespace ZSharp {

/// <summary>
/// Asset loader interface for loading things from persistent storage.
/// i.e. NAND Flash, Hard Disks, etc.
/// </summary>
class AssetLoader {
  public:
  /// <summary>
  /// Don't allow any objects to be constructed for this class.
  /// </summary>
  AssetLoader() = delete;

  /// <summary>
  /// Parse a given file and return the data associated with it.
  /// </summary>
  /// <param name='fileName'>Name of the file to parse.</param>
  /// <returns>A model containing all of its data.</returns>
  template <typename T>
  static Model<T> ParseFile(const std::string& fileName);

  private:
  /// <summary>
  /// Typical block size used for reading from SSDs.
  /// </summary>
  const uint32_t BLOCK_SIZE = 4096;

  /// <summary>
  /// Locale used to read the file as.
  /// </summary>
  const std::locale LOCALE = std::locale("en_US.UTF8");
};
}

#endif
