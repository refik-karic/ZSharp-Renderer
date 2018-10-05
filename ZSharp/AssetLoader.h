#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <string>

#include "Scanner.h"
#include "Model.h"

namespace ZSharp {

/// <summary>
/// Asset loader interface for loading things from persistent storage.
/// i.e. NAND Flash, Hard Disks, etc.
/// </summary>
class AssetLoader {
  public:
  /// <summary>
  /// Default constructor.
  /// </summary>
  AssetLoader() {

  }

  /// <summary>
  /// Parse a given file and return the data associated with it.
  /// </summary>
  /// <param name='fileName'>Absolute path to the file to be parsed.</param>
  /// <returns>A model containing all of its data.</returns>
  template <typename T>
  Model<T> LoadModel(const std::string& fileName) {
    // Create a model which will hold the result of reading from the file.
    Model<T> modelResult;

    // Scan the asset file for data and generate a model from that data.
    Scanner scanner;

    const std::list<Scanner::JsonToken>& tokens(scanner.ScanTokens(fileName));

    if (tokens.size() != 0) {
      for (const auto& token : tokens) {

      }
      // TODO: Process the tokens in here.
    }

    return modelResult;
  }

  private:
};
}

#endif
