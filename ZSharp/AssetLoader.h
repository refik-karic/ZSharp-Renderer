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
  Model<T> ParseFile(const std::string& fileName) {
    // Create a model which will hold the result of reading from the file.
    Model<T> modelResult;

    // Open a stream to the file.
    std::ifstream inputStream(fileName, std::ios::binary);

    // Make sure the file is opened before proceeding.
    if (!inputStream.is_open()) {
      return modelResult;
    }

    // Read the file in blocks and send them for processing.
    char data[mBlockSize];
    mData = (uint8_t*)data;
    uint32_t bytesRead(0);
    uint32_t remainingData(0);
    std::list<JsonValuePair> tokenList;

    // Read the file in blocks and then pass those blocks to the lexical analyzer.
    do {
      // Read a block of data from the file.
      inputStream.read(data, mBlockSize - remainingData);
      bytesRead = static_cast<uint32_t>(inputStream.gcount());

      // Call the analyzer with the following state:
      // 1. A pointer to the beginnning of the buffer.
      // 2. The offset from the end of the buffer when there are remaining bytes, else the beginning of the buffer.
      // 3. The amount of data left anywhere in the buffer.
      // 4. A list to store tokens into.
      if (remainingData > 0) {
        remainingData = ParseTokens(mBlockSize - remainingData,
                                    bytesRead + remainingData,
                                    tokenList);
      }
      else {
        remainingData = ParseTokens(0, bytesRead, tokenList);
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
  static const uint32_t mBlockSize = 4096;

  /// <summary>
  /// Buffer of data for processing a file.
  /// </summary>
  uint8_t* mData;

  /// <summary>
  /// Forward declarations for later use.
  /// </summary>
  enum class JsonTokens;
  struct JsonValuePair;

  /// <summary>
  /// Set of tokens supported by JSON file format.
  /// See this for more information: https://www.json.org/
  /// </summary>
  enum class JsonTokens {
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
  /// Pair of value type and its associated data.
  /// </summary>
  struct JsonValuePair {
    JsonTokens token;
    std::string dataString;
    union {
      bool dataBool;
      double dataFloat;
      int64_t dataInt;
    };
  };

  /// <summary>
  /// Parses tokens from a buffer and populates a list with those tokens.
  /// </summary>
  /// <param name='index'>The index into that buffer for where to start parsing from.</param>
  /// <param name='bytesRemaining'>The amount of bytes left in the buffer to read. The buffer is treated as a circular buffer and will wrap around to complete reading.</param>
  /// <param name='tokens'>A list used to store tokens in.</param>
  /// <returns>The amount of leftover data in the buffer, if any, that cannot be parsed in one pass.</returns>
  uint32_t ParseTokens(uint32_t index, uint32_t bytesRemaining, std::list<JsonValuePair>& tokens);

  /// <summary>
  /// Look ahead for the next token.
  /// </summary>
  /// <param name='symbol'>The ending symbol to look for.</param>
  /// <param name='bufferIndex'>The current index into the buffer.</param>
  /// <param name='lookIndex'>The starting index of look ahead in the buffer.</param>
  /// <param name='end'>The ending index in the buffer.</param>
  /// <returns>The index of the look ahead after the symbol/end of buffer has been reached.</returns>
  uint32_t LookAhead(uint8_t symbol, uint32_t bufferIndex, uint32_t lookIndex, uint32_t end);

  /// <summary>
  /// Shifts data from a given index to the end of the buffer.
  /// </summary>
  /// <param name='data'>The buffer containing data.</param>
  /// <param name='index'>The index read up to thus far. (this can be out of bounds because it is a circular buffer.</param>
  /// <param name='bytesRemaining'>The amount of data to shift over.</param>
  void ShiftData(uint32_t index, uint32_t bytesRemaining);
};
}

#endif
