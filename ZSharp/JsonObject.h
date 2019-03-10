#pragma once

#include <cstdint>

#include <memory>
#include <string>
#include <vector>

namespace ZSharp {

class JsonObject {
  public:

  /// <summary>
  /// Set of tokens supported by JSON file format.
  /// See this for more information: https://www.json.org/
  /// </summary>
  enum class JsonTokenType {
    // Unary tokens.
    OPEN_CURLY_BRACE,
    CLOSE_CURLY_BRACE,
    OPEN_SQUARE_BRACE,
    CLOSE_SQUARE_BRACE,
    COMMA,
    COLON,
    // Literal values.
    STRING,
    NUMBER_FLOAT,
    NUMBER_INT,
    BOOL_VALUE,
    NULL_VALUE
  };

  /// <summary>
  /// Supported values for JSON obbjects.
  /// </summary>
  struct JsonTokenValue {
    std::string dataString;
    union {
      bool dataBool;
      double dataFloat;
      std::int64_t dataInt;
    };
  };

  /// <summary>
  /// Pair of value type and its associated data.
  /// </summary>
  struct JsonToken {
    JsonTokenType token;
    JsonTokenValue value;
  };

  /// <summary>
  /// Represents all the supported JSON value types per the JSON standard.
  /// </summary>
  enum class JsonValueType {
    STRING_VALUE,
    NUMBER_INT_VALUE,
    NUMBER_FLOAT_VALUE,
    OBJECT_VALUE,
    ARRAY_VALUE,
    BOOL_VALUE,
    NULL_VALUE
  };

  /// <summary>
  /// Holds all information regarding JSON keys.
  /// This includes the type of the key as well as its value(s).
  /// </summary>
  struct JsonValue {
    JsonValueType valueType;
    std::string dataString;
    union {
      bool dataBool;
      double dataFloat;
      std::int64_t dataInt;
    };
    std::shared_ptr<JsonObject> dataObject;
    std::vector<JsonValue> dataArray;
  };

  void SetKey(const std::string& key);

  std::string& GetKey();

  void SetValue(const JsonValue& value);

  JsonValue& GetValue();

  private:
  /// <summary>
  /// String key.
  /// </summary>
  std::string mKey;

  /// <summary>
  /// Value associated with key.
  /// </summary>
  JsonValue mValue;
};

}
