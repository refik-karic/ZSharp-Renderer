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
    std::list<JsonValuePair> tokenList;

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
        remainingData = ParseTokens((uint8_t*)buffer, 0, bytesRead, tokenList);
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
    NUMBER
  };

  /// <summary>
  /// Pair of value type and its associated data.
  /// </summary>
  struct JsonValuePair {
    JsonTokens token;
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
  /// <returns>The amount of leftover data in the buffer, if any, that cannot be parsed in one pass.</returns>
  static uint32_t ParseTokens(uint8_t* data, uint32_t index, uint32_t bytesRemaining, std::list<JsonValuePair>& tokens) {
    uint32_t bytesProcessed(0);
    uint32_t lookAhead(0);
    
    while (bytesProcessed < bytesRemaining) {
      JsonValuePair pair;

      switch (data[(index + bytesProcessed) % BLOCK_SIZE]) {
        case static_cast<uint8_t>('{') :
          pair.token = JsonTokens::OPEN_CURLY_BRACE;
          tokens.push_back(pair);
          bytesProcessed++;
          break;
        case static_cast<uint8_t>('}'):
          pair.token = JsonTokens::CLOSE_CURLY_BRACE;
          tokens.push_back(pair);
          bytesProcessed++;
          break;
        case static_cast<uint8_t>('[') :
          pair.token = JsonTokens::OPEN_SQUARE_BRACE;
          tokens.push_back(pair);
          bytesProcessed++;
          break;
        case static_cast<uint8_t>(']') :
          pair.token = JsonTokens::CLOSE_SQUARE_BRACE;
          tokens.push_back(pair);
          bytesProcessed++;
          break;
        case static_cast<uint8_t>(',') :
          pair.token = JsonTokens::COMMA;
          tokens.push_back(pair);
          bytesProcessed++;
          break;
        case static_cast<uint8_t>(':') :
          pair.token = JsonTokens::COLON;
          tokens.push_back(pair);
          bytesProcessed++;
          break;
        case static_cast<uint8_t>('"') :
          pair.token = JsonTokens::STRING;

          // Look ahead until:
          // 1. Another string literal is found.
          // 2. The end of the buffer has been reached.
          lookAhead = bytesProcessed + 1;
          while ((lookAhead < bytesRemaining) &&
                 (data[(index + lookAhead) % BLOCK_SIZE] != static_cast<uint8_t>('"'))) {
            lookAhead++;
          }

          // Check once more if the look ahead position is the end of string.
          if (data[(index + lookAhead) % BLOCK_SIZE] != static_cast<uint8_t>('"')) {
            // Shift the remaining data to the end of the buffer so that it doesn't get overriden by the next read() call.
            ShiftData(data, index + bytesProcessed, bytesRemaining - bytesProcessed);

            // Read another chunk of data to try and find the end of this token.
            return bytesRemaining - bytesProcessed;
          }

          // Create a string object based off of a substring of the literal token.
          {
            std::string temp(lookAhead - bytesProcessed + 1, '\0');

            for (uint32_t dataIndex(bytesProcessed), tempIndex(0); dataIndex < lookAhead; dataIndex++, tempIndex++) {
              temp[tempIndex] = data[dataIndex % BLOCK_SIZE];
            }

            pair.dataString = temp;
          }

          tokens.push_back(pair);
          // Add the delta from the look ahead.
          bytesProcessed = bytesProcessed + (lookAhead - bytesProcessed) + 1;
          break;
        default:
          bytesProcessed++;
      }
    }

    // This should only ever happen if something went wrong.
    // In that case, any data in the existing buffer will be overwritten on the next file read.
    return 0;
  }

  /// <summary>
  /// Shifts data from a given index to the end of the buffer.
  /// </summary>
  /// <param name='data'>The buffer containing data.</param>
  /// <param name='index'>The index read up to thus far. (this can be out of bounds because it is a circular buffer.</param>
  /// <param name='bytesRemaining'>The amount of data to shift over.</param>
  static void ShiftData(uint8_t* data, uint32_t index, uint32_t bytesRemaining) {
    while (bytesRemaining > 0) {
      data[BLOCK_SIZE - bytesRemaining] = data[index % BLOCK_SIZE];
      bytesRemaining--;
      index++;
    }
  }
};
}

#endif
