#include "Scanner.h"

namespace ZSharp {

bool Scanner::ReadFromFile(const std::string& fileName) {
  // Open a stream to the file.
  std::ifstream inputStream(fileName, std::ios::binary);

  // Make sure the file is opened before proceeding.
  if (!inputStream.is_open()) {
    return false;
  }

  // TODO: Look into whether seeking to the end of large files has an impact on performance.
  // We could read and allocate memory as we go but this might be slightly more efficient.

  // Seek to the end of the file.
  inputStream.seekg(0, std::ios::end);

  // Resize the vector once to avoid potential future memory allocations when reading blocks.
  mFileBuffer.resize(static_cast<std::size_t>(inputStream.tellg()));

  // Seek back to the beginnning and start reading.
  inputStream.seekg(0, std::ios::beg);

  // Track how many bytes were read on each iteration.
  std::uint32_t bytesRead(0);

  do {
    // Read a block of data from the file into the next position in the buffer.
    inputStream.read(mFileBuffer.data() + bytesRead, BLOCK_SIZE);
    bytesRead = static_cast<std::uint32_t>(inputStream.gcount());
  } while (bytesRead != 0);

  // Release the resource before finishing up.
  inputStream.close();

  return true;
}

const std::list<Scanner::JsonToken>& Scanner::ScanTokens(const std::string& fileName) {
  if (!ReadFromFile(fileName)) {
    return mTokens;
  }
  
  while (!IsAtEnd()) {
    mStart = mCurrent;
    ScanToken();
  }

  // TODO: Finish implementing this section.

  return mTokens;
}

void Scanner::ScanToken() {
  char c = Advance();

  switch (c) {
    case '{':
      AddToken(JsonTokenType::OPEN_CURLY_BRACE);
      break;
    case '}':
      AddToken(JsonTokenType::CLOSE_CURLY_BRACE);
      break;
    case '[':
      AddToken(JsonTokenType::OPEN_SQUARE_BRACE);
      break;
    case ']':
      AddToken(JsonTokenType::CLOSE_SQUARE_BRACE);
      break;
    case ',':
      AddToken(JsonTokenType::COMMA);
      break;
    case ':':
      AddToken(JsonTokenType::COLON);
      break;
    case '"':
      ScanString();
      break;
    default:
      if (IsDigit(c)) {
        ScanNumber();
      }
      else {
        // TODO: Print out an error here.
      }
  }
}

void Scanner::ScanString() {
  while (Peek() != '"' && !IsAtEnd()) {
    if (Peek() == '\n') {
      mLine++;
    }

    Advance();
  }

  if (IsAtEnd()) {
    // TODO: Handle error here when unterminated string.
    return;
  }

  Advance();

  // Scan a string from the substring of the buffered data.
  // Make sure to ignore the quotes surrounding the string by adjusting the start and end by 1.
  std::string scannedString(mFileBuffer.data() + mStart + 1, mFileBuffer.data() + mCurrent - 1);
  scannedString = scannedString + '\0';

  JsonToken nextToken;
  nextToken.token = JsonTokenType::STRING;
  nextToken.dataString = scannedString;
  AddToken(nextToken);
}

void Scanner::ScanNumber() {
  bool realNumber = false;

  while (IsDigit(Peek())) {
    Advance();
  }

  if (Peek() == '.' && IsDigit(PeekNext())) {
    realNumber = true;

    Advance();

    while (IsDigit(Peek())) {
      Advance();
    }
  }

  // Create a string from the substring of the scanned lexeme.
  std::string scannedString(mFileBuffer.data() + mStart, mFileBuffer.data() + mCurrent);
  scannedString = scannedString + '\0';

  // Create a token based off of the scanned number value.
  JsonToken nextToken;

  if (realNumber) {
    nextToken.token = JsonTokenType::NUMBER_FLOAT;
    nextToken.dataFloat = std::atof(scannedString.c_str());
  }
  else {
    nextToken.token = JsonTokenType::NUMBER_INT;
    nextToken.dataInt = static_cast<std::int64_t>(std::atoll(scannedString.c_str()));
  }

  AddToken(nextToken);
}

}
