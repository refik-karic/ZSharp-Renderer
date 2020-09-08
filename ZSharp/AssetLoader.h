#pragma once

#include <array>
#include <vector>

#include "JsonObject.h"
#include "Model.h"
#include "JsonScanner.h"
#include "Token.h"

namespace ZSharp {

class AssetLoader final {
  public:
  AssetLoader() = delete;

  template <typename T>
  static void LoadModel(const char* fileName, Model<T>& model) {
    // Scan the asset file for data and generate a model from that data.
    JsonScanner scanner;
    JsonObject jsonObject;

    // Convert the output of the scanner to a JSON object and fill it with data.
    std::vector<Token> tokens;
    scanner.ScanTokens(fileName, tokens);

    if(tokens.empty()) {
      return;
    }

    // Fill in the JSON object with data.
    scanner.PopulateJsonObject(jsonObject, tokens);

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
};
}
