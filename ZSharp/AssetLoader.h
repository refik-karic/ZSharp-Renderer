#pragma once

#include <array>
#include <vector>

#include "Model.h"
#include "JsonObject.h"
#include "Scanner.h"

namespace ZSharp {

/// <summary>
/// Asset loader interface for loading things from persistent storage.
/// i.e. NAND Flash, Hard Disks, etc.
/// </summary>
class AssetLoader {
  public:
  AssetLoader() = delete;

  template <typename T>
  static void LoadModel(const char* fileName, Model<T>& model) {
    // Scan the asset file for data and generate a model from that data.
    Scanner scanner;
    JsonObject jsonObject;

    // Convert the output of the scanner to a JSON object and fill it with data.
    std::vector<JsonObject::JsonToken> tokens;
    scanner.ScanTokens(fileName, tokens);

    if(tokens.empty()){
      return;
    }

    // Fill in the JSON object with data.
    PopulateJsonObject(jsonObject, tokens);

    // Make room for the amount of meshes loaded in the requested model.
    model = Model<T>(jsonObject.GetValue().dataArray.size());
    
    // Loop over each mesh in the model asset file.
    for (std::size_t meshIndex = 0; meshIndex < jsonObject.GetValue().dataArray.size(); ++meshIndex) {
      auto indicies = jsonObject.GetValue().dataArray[meshIndex].dataObject.get()->GetValue().dataArray[0].dataObject.get();
      auto verticies = jsonObject.GetValue().dataArray[meshIndex].dataObject.get()->GetValue().dataArray[1].dataObject.get();

      Mesh<T>& mesh = model[meshIndex];

      // Copy all of the JSON array tokens to a raw continuous array in memory.
      std::vector<T> vertData(verticies->GetValue().dataArray.size());
      for (std::size_t i = 0; i < vertData.size(); ++i) {
        vertData[i] = static_cast<T>(verticies->GetValue().dataArray[i].dataFloat);
      }

      // Set the vertex data and number of faces/triangles in the mesh.
      mesh.SetData(vertData.data(), vertData.size(), indicies->GetValue().dataArray.size());

      // Set each individual triangle with its indicies into the mesh vertex table.
      for (std::size_t triIndex = 0; triIndex < indicies->GetValue().dataArray.size(); ++triIndex) {
        std::array<size_t, 3> triIndicies;
        triIndicies[0] = static_cast<std::size_t>(indicies->GetValue().dataArray[triIndex].dataArray[0].dataInt);
        triIndicies[1] = static_cast<std::size_t>(indicies->GetValue().dataArray[triIndex].dataArray[1].dataInt);
        triIndicies[2] = static_cast<std::size_t>(indicies->GetValue().dataArray[triIndex].dataArray[2].dataInt);
        mesh.SetTriangle(triIndicies, triIndex);
      }
    }
  }

  private:

  /// <summary>
  /// Fills in a JSON object with data, given by a list of tokens from the scanner.
  /// </summary>
  /// <param name='jsonObject'>The JSON object to fill in with data.</param>
  /// <param name='tokenList'>A list of tokens scanned in by the parser.</param>
  static void PopulateJsonObject(JsonObject& jsonObject, std::vector<JsonObject::JsonToken>& tokenList);

  /// <summary>
  /// Recursively add data to a JSON object.
  /// </summary>
  /// <param name='jsonObject'>The current object level.</param>
  /// <param name='begin'>An iterator pointing to the current index.</param>
  /// <param name='end'>An iterator pointing the end.</param>
  /// <param name='isArray'>Flag for representing whether or not the current call should store upcoming values in the included array.</param>
  static void ProcessJsonData(JsonObject& jsonObject, std::vector<JsonObject::JsonToken>::iterator& begin, std::vector<JsonObject::JsonToken>::iterator& end, bool isArray);
};
}
