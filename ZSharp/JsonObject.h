#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include <string>

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
    BOOLEAN,
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
      int64_t dataInt;
    };
  };

  /// <summary>
  /// Pair of value type and its associated data.
  /// </summary>
  struct JsonToken {
    JsonTokenType token;
    JsonTokenValue value;
  };



  private:
  /// <summary>
  /// String key.
  /// </summary>
  std::string mKey;

  /// <summary>
  /// TODO: Find out how to recursively store values and handle objects without any keys.
  /// </summary>
  //JsonValue mValue;

};

}

#endif
