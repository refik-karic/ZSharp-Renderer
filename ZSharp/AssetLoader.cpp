#include "AssetLoader.h"

namespace ZSharp {

void AssetLoader::PopulateJsonObject(JsonObject & jsonObject, std::list<JsonObject::JsonToken>& tokenList) {
  // Chop off the first open curly brace before beginning to process the rest.
  // Otherwise the first object would assume it is a value for a key that does not actually exist.
  for (auto iter = tokenList.begin(); iter != tokenList.end(); ++iter) {
    if (iter->token == JsonObject::JsonTokenType::OPEN_CURLY_BRACE) {
      // Skip over that first curly brace.
      ++iter;

      auto tmpEndIter = tokenList.end();
      ProcessJsonData(jsonObject, iter, tmpEndIter, false);
      return;
    }
  }
}

void AssetLoader::ProcessJsonData(JsonObject& jsonObject, std::list<JsonObject::JsonToken>::iterator& begin, std::list<JsonObject::JsonToken>::iterator& end, bool isArray) {
  bool keyDetected = false;

  // Search the remaining items in the list for data.
  for (; begin != end; begin++) {
    JsonObject::JsonValue tmpValue;

    switch (begin->token) {
      case JsonObject::JsonTokenType::OPEN_CURLY_BRACE:
        {
          tmpValue.valueType = JsonObject::JsonValueType::OBJECT_VALUE;
          
          begin++;

          // Nested object, fill it in and copy the pointer to the active object.
          auto tmpObject(std::make_shared<JsonObject>());
          ProcessJsonData(*tmpObject, begin, end, false);
          tmpValue.dataObject = tmpObject;

          if (isArray) {
            jsonObject.GetValue().dataArray.push_back(tmpValue);
          }
          else {
            jsonObject.SetValue(tmpValue);
          }
        }
        break;
      case JsonObject::JsonTokenType::OPEN_SQUARE_BRACE:
        tmpValue.valueType = JsonObject::JsonValueType::ARRAY_VALUE;
        begin++;

        // Special case for multi-dimensional arrays.        
        if (isArray) {
          // Create a temporary JSON object to hold additional dimensions.
          JsonObject tmpObject;

          // Grab the following data and stuff it into the temporary's array.
          ProcessJsonData(tmpObject, begin, end, true);

          // Append the contents of the temporary array to the existing one.
          tmpValue.dataArray = tmpObject.GetValue().dataArray;
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        }
        else {
          jsonObject.GetValue().valueType = tmpValue.valueType;
          ProcessJsonData(jsonObject, begin, end, true);
        }
        break;
      case JsonObject::JsonTokenType::CLOSE_SQUARE_BRACE:
      case JsonObject::JsonTokenType::CLOSE_CURLY_BRACE:
        // Go back up a level in the call chain.
        return;
      case JsonObject::JsonTokenType::STRING:
        if (!isArray && !keyDetected) {
          keyDetected = true;
          jsonObject.SetKey(begin->value.dataString);
        }
        else {
          tmpValue.valueType = JsonObject::JsonValueType::STRING_VALUE;
          tmpValue.dataString = begin->value.dataString;
          
          if (isArray) {
            jsonObject.GetValue().dataArray.push_back(tmpValue);
          }
          else {
            jsonObject.SetValue(tmpValue);
          }
        }
        break;
      case JsonObject::JsonTokenType::NUMBER_INT:
        tmpValue.valueType = JsonObject::JsonValueType::NUMBER_INT_VALUE;
        tmpValue.dataInt = begin->value.dataInt;

        if (isArray) {
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        }
        else {
          jsonObject.SetValue(tmpValue);
        }

        break;
      case JsonObject::JsonTokenType::NUMBER_FLOAT:
        tmpValue.valueType = JsonObject::JsonValueType::NUMBER_FLOAT_VALUE;
        tmpValue.dataFloat = begin->value.dataFloat;

        if (isArray) {
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        }
        else {
          jsonObject.SetValue(tmpValue);
        }

        break;
      case JsonObject::JsonTokenType::BOOL_VALUE:
        tmpValue.valueType = JsonObject::JsonValueType::BOOL_VALUE;
        tmpValue.dataBool = begin->value.dataBool;
        
        if (isArray) {
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        } else {
          jsonObject.SetValue(tmpValue);
        }

        break;
    }
  }
}

}
