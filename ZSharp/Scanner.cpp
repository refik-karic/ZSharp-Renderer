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
  JsonToken nextToken;
  nextToken.token = JsonTokenType::STRING;
  nextToken.dataString = scannedString;
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
  JsonToken nextToken;

  // Convert the number to its appropriate type.
  if (realNumber) {
    nextToken.token = JsonTokenType::NUMBER_FLOAT;
    nextToken.dataFloat = std::atof(scannedString.c_str());
  }
  else {
    nextToken.token = JsonTokenType::NUMBER_INT;
    nextToken.dataInt = static_cast<std::int64_t>(std::atoll(scannedString.c_str()));
  }

  // Add it to the list of tokens.
  AddToken(nextToken);
}

}
