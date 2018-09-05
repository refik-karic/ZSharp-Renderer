#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <fstream>
#include <locale>
#include <string>

#include "Model.h"

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
  /// <param name='fileName'>Absolute path to the file to be parsed.</param>
  /// <returns>A model containing all of its data.</returns>
  template <typename T>
  static Model<T> ParseFile(const std::string& fileName) {
    // Create a model which will hold the result of reading from the file.
    Model<T> modelResult;

    // Open a stream to the file.
    std::ifstream inputStream(fileName, std::ios::binary);

    // Make sure the file is opened before proceeding.
    if (!inputStream.is_open()) {
      return modelResult;
    }

    // Read the file in blocks and send them for processing.
    char buffer[BLOCK_SIZE];
    uint32_t bytesRead(0);

    // Read the file in blocks and then pass those blocks to the lexical analyzer.
    do {
      inputStream.read(buffer, BLOCK_SIZE);
      bytesRead = static_cast<uint32_t>(inputStream.gcount());

      LexicalAnaylze((uint8_t*)buffer, bytesRead);

    } while (bytesRead == BLOCK_SIZE);

    // Release the resource before finishing up.
    inputStream.close();

    return modelResult;
  }

  private:
  /// <summary>
  /// Typical block size used for reading from SSDs.
  /// </summary>
  static const uint32_t BLOCK_SIZE = 4096;

  /// <summary>
  /// Forward declarations for later use.
  /// </summary>
  enum class Tokens;
  enum class JsonValueType;
  struct JsonValue;
  struct JsonObject;

  /// <summary>
  /// Set of tokens supported by JSON file format.
  /// See this for more information: https://www.json.org/
  /// </summary>
  enum class JsonTokens {
    OPEN_CURLY_BRACE = '{',
    CLOSE_CURLY_BRACE = '}',
    OPEN_SQUARE_BRACE = '[',
    CLOSE_SQUARE_BRACE = ']',
    COMMA = ',',
    COLON = ':',
    STRING = '"'
  };

  enum class JsonValueType {
    STRING_VALUE,
    NUMBER_INTEGER_VALUE,
    NUMBER_FLOAT_VALUE,
    OBJECT_VALUE,
    ARRAY_VALUE,
    BOOL_VALUE,
    NULL_VALUE
  };

  struct JsonValue {
    JsonValueType type;
    
    char* string;
    uint64_t lengthArray;

    uint64_t numberInteger;
    double numberFloat;

    JsonObject* object;

    JsonValue* arrValues;

    bool flag;
    bool null;
  };

  struct JsonObject {
    char* key;
    uint32_t length;
    JsonValue value;
  };

  static void LexicalAnaylze(uint8_t* data, uint32_t size) {
    uint32_t position(0);

    while (position < size) {
      NextToken(data, position, size);
    }
  }

  static uint8_t NextToken(uint8_t*& data, uint32_t& index, uint32_t size) {
    while (index < size) {
      switch (data[index]) {
        case static_cast<uint8_t>(JsonTokens::OPEN_CURLY_BRACE) :
          data += index;
          return static_cast<uint8_t>(JsonTokens::OPEN_CURLY_BRACE);
        case static_cast<uint8_t>(JsonTokens::STRING):
          // TODO: Finish parsing the token value at the given string.

        default:
          index++;
      }
    }

    return 0;
  }
};
}

#endif
