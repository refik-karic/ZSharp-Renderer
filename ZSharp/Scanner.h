#pragma once

#include <cstdint>
#include <cstdlib>

#include <vector>

#include "Token.h"

// Thank you to Bob Nystrom
// http://www.craftinginterpreters.com/
// This implementation borrows heavily from his work. Its been a long time since I had to implement a Recursive Descent Parser so thank you for publishing this online.
// Unlike his implementation, I don't actually execute the tokens afterwards. I mainly just rely on the scanner to tokenize the data and then use that to populate my internal data structures.

namespace ZSharp {

class Scanner {
  public:

  Scanner() {

  }

  void ScanTokens(const char* fileName, std::vector<Token>& tokens);

  protected:
  std::size_t mStart = 0;
  
  std::size_t mCurrent = 0;
  
  std::size_t mLine = 0;

  bool IsAtEnd(std::vector<char>& fileBuffer);

  virtual void ScanToken(std::vector<Token>& tokens, std::vector<char>& fileBuffer) = 0;

  char Advance(std::vector<char>& fileBuffer);

  char Peek(std::vector<char>& fileBuffer);

  void AddToken(Token& token, std::vector<Token>& tokens);

  char PeekNext(std::vector<char>& fileBuffer);
};

}
