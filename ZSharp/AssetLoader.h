#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <list>
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

  template <typename T>
  void LoadModel(const std::string& fileName, Model<T>& model) {
    // Scan the asset file for data and generate a model from that data.
    Scanner scanner;
    JsonObject jsonObject;

    // Convert the output of the scanner to a JSON object and fill it with data.
    auto& tokenList = scanner.ScanTokens(fileName);

    // Fill in the JSON object with data.
    PopulateJsonObject(jsonObject, tokenList);

    // Populate model data from JSON object.
    
    // Loop over each mesh in the model asset file.
    for (std::size_t i; i < jsonObject.GetValue().dataArray.size(); i++) {
      const auto& meshData = jsonObject.GetValue().dataArray;
      // TODO: Finish this implementation later.
      // Look for an efficient way to copy the data over.
      // Avoid data duplication by having the triangles contain indicies into the raw array of the mesh.
    }
  }

  private:

  /// <summary>
  /// Fills in a JSON object with data, given by a list of tokens from the scanner.
  /// </summary>
  /// <param name='jsonObject'>The JSON object to fill in with data.</param>
  /// <param name='tokenList'>A list of tokens scanned in by the parser.</param>
  void PopulateJsonObject(JsonObject& jsonObject, std::list<JsonObject::JsonToken>& tokenList);

  /// <summary>
  /// Recursively add data to a JSON object.
  /// </summary>
  /// <param name='jsonObject'>The current object level.</param>
  /// <param name='begin'>An iterator pointing to the current index.</param>
  /// <param name='end'>An iterator pointing the end.</param>
  /// <param name='isArray'>Flag for representing whether or not the current call should store upcoming values in the included array.</param>
  void ProcessJsonData(JsonObject& jsonObject, std::list<JsonObject::JsonToken>::iterator& begin, std::list<JsonObject::JsonToken>::iterator& end, bool isArray);
};
}

#endif
