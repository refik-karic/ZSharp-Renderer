#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <string>

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
  /// <returns>TBD</returns>
  void ParseFile(const std::string& fileName);
};
}

#endif
