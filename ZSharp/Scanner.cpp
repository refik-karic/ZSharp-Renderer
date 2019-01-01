#include "Scanner.h"

namespace ZSharp {

bool Scanner::ReadFromFile(const std::string& fileName) {
  // Open a stream to the file.
  std::ifstream inputStream(fileName, std::ios::binary);

  // Make sure the file is opened before proceeding.
  if (!inputStream.is_open()) {
    return false;
  }

  // Leave it to the standard library to worry about the most efficient platform specific way to read the file.
  mFileBuffer = std::vector<char>(std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>());

  return true;
}

std::list<JsonObject::JsonToken>& Scanner::ScanTokens(const std::string& fileName) {
  // Clear any previous file contents.
  mTokens.clear();
  
  // Read the entire file into memory.
  // Return an empty list of tokens if nothing was read.
  if (!ReadFromFile(fileName)) {
    return mTokens;
  }
  
  // Scan the tokens one by one until the entire file has been processed.
  while (!IsAtEnd()) {
    mStart = mCurrent;
    ScanToken();
  }

  // Return the populated list of tokens.
  return mTokens;
}

void Scanner::ScanToken() {
  // Fetch the next character in the buffer.
  char c = Advance();

  // Check which token category it falls into.
  switch (c) {
    case '{':
      AddToken(JsonObject::JsonTokenType::OPEN_CURLY_BRACE);
      break;
    case '}':
      AddToken(JsonObject::JsonTokenType::CLOSE_CURLY_BRACE);
      break;
    case '[':
      AddToken(JsonObject::JsonTokenType::OPEN_SQUARE_BRACE);
      break;
    case ']':
      AddToken(JsonObject::JsonTokenType::CLOSE_SQUARE_BRACE);
      break;
    case ',':
      AddToken(JsonObject::JsonTokenType::COMMA);
      break;
    case ':':
      AddToken(JsonObject::JsonTokenType::COLON);
      break;
    case '"':
      ScanString();
      break;
    case '\n':
      mLine++;
    default:
      // Check if the next token is a number.
      // Numbers can have a preceeding minus sign indicating negative values.
      if ((c == '-' && IsDigit(Peek())) || IsDigit(c)) {
        ScanNumber();
      }
      else {
        std::cout << "Unknown token at position: " << mCurrent << ", Line: " << mLine << std::endl;
      }
  }
}

void Scanner::ScanString() {
  // Keep scanning the buffer until the end of the current string has been reached.
  // Strings can span over multiple lines.
  while (Peek() != '"' && !IsAtEnd()) {
    if (Peek() == '\n') {
      mLine++;
    }

    Advance();
  }

  // Make sure the string has terminated before the end of the file was reached.
  if (IsAtEnd()) {
    std::cout << "Non-terminated String detected at position: " << mCurrent << ", Line: " << mLine << std::endl;
    return;
  }

  // Absorb the ending quote.
  Advance();

  // Scan a string from the substring of the buffered data.
  // Make sure to ignore the quotes surrounding the string by adjusting the start and end by 1.
  std::string scannedString(mFileBuffer.data() + mStart + 1, mFileBuffer.data() + mCurrent - 1);
  scannedString = scannedString + '\0';

  // Add the string to the list of tokens.
  JsonObject::JsonToken nextToken;
  nextToken.token = JsonObject::JsonTokenType::STRING;
  JsonObject::JsonTokenValue nextValue;
  nextValue.dataString = scannedString;
  nextToken.value = nextValue;
  AddToken(nextToken);
}

void Scanner::ScanNumber() {
  // Used to indicate whether or not the current number is a real number with fractional digits.
  bool realNumber = false;

  // Keep scanning until no more digits are left in the current lexeme.
  while (IsDigit(Peek())) {
    Advance();
  }

  // Check for real numbers.
  if (Peek() == '.' && IsDigit(PeekNext())) {
    realNumber = true;
    Advance();

    // Read the rest of the fractional digits.
    while (IsDigit(Peek())) {
      Advance();
    }
  }

  // Create a string from the substring of the scanned lexeme.
  std::string scannedString(mFileBuffer.data() + mStart, mFileBuffer.data() + mCurrent);
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
  AddToken(nextToken);
}

}
