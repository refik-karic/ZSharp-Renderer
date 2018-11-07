#include "AssetLoader.h"

namespace ZSharp {

JsonObject AssetLoader::PopulateJsonFromTokens(const std::list<JsonObject::JsonToken>& tokens) {
  JsonObject jsonObject;
  
  if (tokens.size() != 0) {
    for (const auto& tokenType : tokens) {
      if (tokenType.token == JsonObject::JsonTokenType::OPEN_CURLY_BRACE) {
        // TODO: Iterate through the list of tokens until a matching curly brace is found.
      }
    }
  }

  return jsonObject;
}

}
