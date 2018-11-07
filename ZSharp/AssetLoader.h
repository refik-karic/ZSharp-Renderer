#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <string>

#include "JsonObject.h"
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

    // Convert the output of the scanner to a JSON object and fill it with data.
    JsonObject jsonObject(PopulateJsonFromTokens(scanner.ScanTokens(fileName)));

    // TODO: Populate model data from JSON object.


    // Return the populated model to the caller.
    return modelResult;
  }

  private:

  /// <summary>
  /// Populate a JSON object with tokens from the scanner.
  /// </summary>
  /// <param name = 'tokens'>A list of tokens from the scanner to process.</param>
  /// <returns>A new JSON object containing the parsed data.</returns>
  JsonObject PopulateJsonFromTokens(const std::list<JsonObject::JsonToken>& tokens);
};
}

#endif
