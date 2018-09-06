#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <fstream>
#include <list>
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
    uint32_t remainingData(0);
    std::list<JsonValueType> tokenList;

    // Read the file in blocks and then pass those blocks to the lexical analyzer.
    do {
      // Read a block of data from the file.
      inputStream.read(buffer, BLOCK_SIZE - remainingData);
      bytesRead = static_cast<uint32_t>(inputStream.gcount());

      // Call the analyzer with the following state:
      // 1. A pointer to the beginnning of the buffer.
      // 2. The offset from the end of the buffer when there are remaining bytes, else the beginning of the buffer.
      // 3. The amount of data left anywhere in the buffer.
      // 4. A list to store tokens into.
      if (remainingData > 0) {
        remainingData = ParseTokens((uint8_t*)buffer,
                                       BLOCK_SIZE - remainingData,
                                       bytesRead + remainingData,
                                       tokenList);
      }
      else {
        remainingData = ParseTokens((uint8_t*)buffer, 0, bytesRead);
      }
    } while (bytesRead != 0);

    if (remainingData > 0) {
      // TODO: Handle an error where the buffer was filled up, no data was read, and there is still stuff left to be parsed.
    }

    // Release the resource before finishing up.
    inputStream.close();

    // TODO: Process the stream of tokens from the file and generate a model from it.


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
  enum class JsonValueType;
  enum class JsonTokens;
  struct JsonValuePair;

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

  /// <summary>
  /// List of different values supported by json.
  /// </summary>
  enum class JsonValueType {
    STRING_VALUE,
    NUMBER_INTEGER_VALUE,
    NUMBER_FLOAT_VALUE,
    OBJECT_VALUE,
    ARRAY_VALUE,
    BOOL_VALUE,
    NULL_VALUE
  };

  /// <summary>
  /// Pair of value type and its associated data.
  /// </summary>
  struct JsonValuePair {
    JsonValueType type;
    std::string dataString;
    double dataFloat;
    uint64_t dataInt;
  };

  /// <summary>
  /// Parses tokens from a buffer and populates a list with those tokens.
  /// </summary>
  /// <param name='data'>A pointer to the beginning of a buffer of data.</param>
  /// <param name='index'>The index into that buffer for where to start parsing from.</param>
  /// <param name='bytesRemaining'>The amount of bytes left in the buffer to read. The buffer is treated as a circular buffer and will wrap around to complete reading.</param>
  /// <param name='tokens'>A list used to store tokens in.</param>
  /// <returns>The amount of leftover data in the buffer, if any, that cannot be parsed in one pass.<returns>
  static uint32_t ParseTokens(uint8_t* data, uint32_t index, uint32_t bytesRemaining, std::list<JsonValuePair>& tokens) {
    uint32_t bytesProcessed(0);
    
    // TODO: Finish up this implementation.
    while (bytesProcessed < bytesRemaining) {
      switch (data[(index + bytesProcessed) % BLOCK_SIZE]) {
        case static_cast<uint8_t>(JsonTokens::OPEN_CURLY_BRACE) :
          break;
        default:
          bytesProcessed++;
      }
    }

    // Return the amount of data leftover in the buffer.
    return bytesRemaining - bytesProcessed;
  }
};
}

#endif
