#include "Scanner.h"

#include <fstream>
#include <iostream> // TODO: Replace this with some kind of global logger singleton.
#include <string>

namespace ZSharp {

void Scanner::ReadFromFile(const char* fileName, std::vector<char>& fileBuffer) {
  // Open a stream to the file.
  std::ifstream inputStream(fileName, std::ios::binary);

  // Make sure the file is opened before proceeding.
  if (!inputStream.is_open()) {
    return;
  }

  // Leave it to the standard library to worry about the most efficient platform specific way to read the file.
  fileBuffer = std::vector<char>(std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>());
}

void Scanner::ScanTokens(const char* fileName, std::vector<JsonObject::JsonToken>& tokens) {
  // Read the entire file into memory.
  // Return an empty list of tokens if nothing was read.
  std::vector<char> fileContents;
  ReadFromFile(fileName, fileContents);

  if(fileContents.empty()) {
    return;
  }
  
  // Scan the tokens one by one until the entire file has been processed.
  while (!IsAtEnd(fileContents)) {
    mStart = mCurrent;
    ScanToken(tokens, fileContents);
  }
}

void Scanner::ScanToken(std::vector<JsonObject::JsonToken>& tokens, std::vector<char>& fileBuffer) {
  // Fetch the next character in the buffer.
  char c = Advance(fileBuffer);

  // Check which token category it falls into.
  switch (c) {
    case '{':
      AddToken(JsonObject::JsonTokenType::OPEN_CURLY_BRACE, tokens);
      break;
    case '}':
      AddToken(JsonObject::JsonTokenType::CLOSE_CURLY_BRACE, tokens);
      break;
    case '[':
      AddToken(JsonObject::JsonTokenType::OPEN_SQUARE_BRACE, tokens);
      break;
    case ']':
      AddToken(JsonObject::JsonTokenType::CLOSE_SQUARE_BRACE, tokens);
      break;
    case ',':
      AddToken(JsonObject::JsonTokenType::COMMA, tokens);
      break;
    case ':':
      AddToken(JsonObject::JsonTokenType::COLON, tokens);
      break;
    case '"':
      ScanString(tokens, fileBuffer);
      break;
    case '\n':
      mLine++;
    default:
      // Check if the next token is a number.
      // Numbers can have a preceeding minus sign indicating negative values.
      if ((c == '-' && IsDigit(Peek(fileBuffer))) || IsDigit(c)) {
        ScanNumber(tokens, fileBuffer);
      }
      else {
        std::cout << "Unknown token at position: " << mCurrent << ", Line: " << mLine << std::endl;
      }
  }
}

void Scanner::ScanString(std::vector<JsonObject::JsonToken>& tokens, std::vector<char>& fileBuffer) {
  // Keep scanning the buffer until the end of the current string has been reached.
  // Strings can span over multiple lines.
  while (Peek(fileBuffer) != '"' && !IsAtEnd(fileBuffer)) {
    if (Peek(fileBuffer) == '\n') {
      mLine++;
    }

    Advance(fileBuffer);
  }

  // Make sure the string has terminated before the end of the file was reached.
  if (IsAtEnd(fileBuffer)) {
    std::cout << "Non-terminated String detected at position: " << mCurrent << ", Line: " << mLine << std::endl;
    return;
  }

  // Absorb the ending quote.
  Advance(fileBuffer);

  // Scan a string from the substring of the buffered data.
  // Make sure to ignore the quotes surrounding the string by adjusting the start and end by 1.
  std::string scannedString(fileBuffer.data() + mStart + 1, fileBuffer.data() + mCurrent - 1);
  scannedString = scannedString + '\0';

  // Add the string to the list of tokens.
  JsonObject::JsonToken nextToken;
  nextToken.token = JsonObject::JsonTokenType::STRING;
  JsonObject::JsonTokenValue nextValue;
  nextValue.dataString = scannedString;
  nextToken.value = nextValue;
  AddToken(nextToken, tokens);
}

void Scanner::ScanNumber(std::vector<JsonObject::JsonToken>& tokens, std::vector<char>& fileBuffer) {
  // Used to indicate whether or not the current number is a real number with fractional digits.
  bool realNumber = false;

  // Keep scanning until no more digits are left in the current lexeme.
  while (IsDigit(Peek(fileBuffer))) {
    Advance(fileBuffer);
  }

  // Check for real numbers.
  if (Peek(fileBuffer) == '.' && IsDigit(PeekNext(fileBuffer))) {
    realNumber = true;
    Advance(fileBuffer);

    // Read the rest of the fractional digits.
    while (IsDigit(Peek(fileBuffer))) {
      Advance(fileBuffer);
    }
  }

  // Create a string from the substring of the scanned lexeme.
  std::string scannedString(fileBuffer.data() + mStart, fileBuffer.data() + mCurrent);
  scannedString = scannedString + '\0';

  // Create a token based off of the scanned number value.
  JsonObject::JsonToken nextToken;
  JsonObject::JsonTokenValue nextValue;

  // Convert the number to its appropriate type.
  if (realNumber) {
    nextToken.token = JsonObject::JsonTokenType::NUMBER_FLOAT;
    nextValue.dataFloat = std::atof(scannedString.c_str());
  }
  else {
    nextToken.token = JsonObject::JsonTokenType::NUMBER_INT;
    nextValue.dataInt = static_cast<std::int64_t>(std::atoll(scannedString.c_str()));
  }

  // Add it to the list of tokens.
  nextToken.value = nextValue;
  AddToken(nextToken, tokens);
}

bool Scanner::IsAtEnd(std::vector<char>& fileBuffer) {
  return mCurrent >= fileBuffer.size();
}

char Scanner::Advance(std::vector<char>& fileBuffer) {
  mCurrent++;
  return fileBuffer[mCurrent - 1];
}

char Scanner::Peek(std::vector<char>& fileBuffer) {
  if (IsAtEnd(fileBuffer)) {
    return '\0';
  } else {
    return fileBuffer[mCurrent];
  }
}

void Scanner::AddToken(JsonObject::JsonTokenType token, std::vector<JsonObject::JsonToken>& tokens) {
  JsonObject::JsonToken nextToken;
  nextToken.token = token;
  tokens.push_back(nextToken);
}

void Scanner::AddToken(JsonObject::JsonToken token, std::vector<JsonObject::JsonToken>& tokens) {
  tokens.push_back(token);
}

bool Scanner::IsDigit(char letter) {
  return letter >= '0' && letter <= '9';
}

char Scanner::PeekNext(std::vector<char>& fileBuffer) {
  if (mCurrent + 1 >= fileBuffer.size()) {
    return '\0';
  } else {
    return fileBuffer[mCurrent + 1];
  }
}

}
