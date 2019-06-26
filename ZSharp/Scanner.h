#pragma once

#include <cstdint>
#include <cstdlib>

#include <vector>

#include "JsonObject.h"

// Thank you to Bob Nystrom
// http://www.craftinginterpreters.com/
// This implementation borrows heavily from his work. Its been a long time since I had to implement a Recursive Descent Parser so thank you for publishing this online.
// Unlike his implementation, I don't actually execute the tokens afterwards. I mainly just rely on the scanner to tokenize the data and then use that to populate my internal data structures.

namespace ZSharp {

class Scanner final {
  public:

  Scanner() {

  }

  /// <summary>
  /// Scans tokens from a file and populates a list with those tokens.
  /// </summary>
  /// <param name='fileName'>The name of the file to scan for tokens.</param>
  /// <returns>
  /// A reference to the list of scanned tokens.
  /// The list will be empty if there were errors when scanning the requested file.
  /// </returns>
  void ScanTokens(const char* fileName, std::vector<JsonObject::JsonToken>& tokens);

  private:
  /// <summary>
  /// Keeps track of the start of the current lexeme.
  /// </summary>
  std::size_t mStart = 0;
  
  /// <summary>
  /// Keep track of the current position in the current lexeme.
  /// </summary>
  std::size_t mCurrent = 0;
  
  /// <summary>
  /// Keeps track of which line the scanner is on for reporting errors.
  /// </summary>
  std::size_t mLine = 0;

  /// <summary>
  /// Read the entire file into memory and store it in the buffer.
  /// </summary>
  /// <param name='fileName'>Name of the file to open.</param>
  /// <param name='buffer'>The buffer to hold the contents of the file in memory.</param>
  /// <returns>True if successfully read from the file, False otherwise.</returns>
  void ReadFromFile(const char* fileName, std::vector<char>& fileBuffer);

  /// <summary>
  /// Check whether or not the scanner has reached the end of the file buffer.
  /// </summary>
  /// <returns>True if end has been reached, False otherwise.</returns>
  bool IsAtEnd(std::vector<char>& fileBuffer);

  /// <summary>
  /// Scan the next token into the list.
  /// </summary>
  void ScanToken(std::vector<JsonObject::JsonToken>& tokens, std::vector<char>& fileBuffer);

  /// <summary>
  /// Advance the scanners position in the current lexeme.
  /// </summary>
  /// <returns>The character at the current position prior to advancing.</returns>
  char Advance(std::vector<char>& fileBuffer);

  /// <summary>
  /// Peek into which character is at the current position in the buffer.
  /// </summary>
  /// <returns>The character at the current position, NULL if the end of buffer has been reached.</returns>
  char Peek(std::vector<char>& fileBuffer);

  /// <summary>
  /// Add a non-data token to the list.
  /// </summary>
  /// <param name='token'>The type of token to add.</param>
  void AddToken(JsonObject::JsonTokenType token, std::vector<JsonObject::JsonToken>& tokens);

  /// <summary>
  /// Add a token with some data to the list.
  /// </summary>
  /// <param name='token'>The token containing its data member.</param>
  void AddToken(JsonObject::JsonToken token, std::vector<JsonObject::JsonToken>& tokens);

  /// <summary>
  /// Scan a string literal token into the list.
  /// </summary>
  void ScanString(std::vector<JsonObject::JsonToken>& tokens, std::vector<char>& fileBuffer);

  /// <summary>
  /// Checks whether or not the current ASCII character is a digit.
  /// </summary>
  /// <param name='letter'>The letter to checker.</param>
  /// <returns>True if it is a digit, False otherwise.</returns>
  bool IsDigit(char letter);

  /// <summary>
  /// Scans the next lexeme for a number token.
  /// </summary>
  void ScanNumber(std::vector<JsonObject::JsonToken>& tokens, std::vector<char>& fileBuffer);

  /// <summary>
  /// Lookahead of one for parsing longer lexemes.
  /// </summary>
  /// <returns>The character two positions ahead in the buffer.</returns>
  char PeekNext(std::vector<char>& fileBuffer);
};

}
