#include "JsonScanner.h"

#include <iostream>
#include <string>

#include "JsonObject.h"

namespace ZSharp {
void JsonScanner::PopulateJsonObject(JsonObject& jsonObject, std::vector<Token>& tokens) {
  // Chop off the first open curly brace before beginning to process the rest.
  // Otherwise the first object would assume it is a value for a key that does not actually exist.
  for (auto iter = tokens.begin(); iter != tokens.end(); ++iter) {
    if (iter->GetType() == TokenType::OPEN_CURLY_BRACE) {
      // Skip over that first curly brace.
      ++iter;

      auto tmpEndIter = tokens.end();
      ProcessJsonData(jsonObject, iter, tmpEndIter, false);
      return;
    }
  }
}

void JsonScanner::ProcessJsonData(JsonObject& jsonObject, std::vector<Token>::iterator& begin, std::vector<Token>::iterator& end, bool isArray) {
  bool keyDetected = false;

  // Search the remaining items in the list for data.
  for (; begin != end; begin++) {
    JsonObject::JsonValue tmpValue;

    switch (begin->GetType()) {
      case TokenType::OPEN_CURLY_BRACE:
      {
        tmpValue.valueType = JsonObject::JsonValueType::OBJECT_VALUE;

        begin++;

        // Nested object, fill it in and copy the pointer to the active object.
        auto tmpObject(std::make_shared<JsonObject>());
        ProcessJsonData(*tmpObject, begin, end, false);
        tmpValue.dataObject = tmpObject;

        if (isArray) {
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        }
        else {
          jsonObject.SetValue(tmpValue);
        }
      }
      break;
      case TokenType::OPEN_SQUARE_BRACE:
        tmpValue.valueType = JsonObject::JsonValueType::ARRAY_VALUE;
        begin++;

        // Special case for multi-dimensional arrays.        
        if (isArray) {
          // Create a temporary JSON object to hold additional dimensions.
          JsonObject tmpObject;

          // Grab the following data and stuff it into the temporary's array.
          ProcessJsonData(tmpObject, begin, end, true);

          // Append the contents of the temporary array to the existing one.
          tmpValue.dataArray = tmpObject.GetValue().dataArray;
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        }
        else {
          jsonObject.GetValue().valueType = tmpValue.valueType;
          ProcessJsonData(jsonObject, begin, end, true);
        }
        break;
      case TokenType::CLOSE_SQUARE_BRACE:
      case TokenType::CLOSE_CURLY_BRACE:
        // Go back up a level in the call chain.
        return;
      case TokenType::STRING:
        if (!isArray && !keyDetected) {
          keyDetected = true;
          jsonObject.SetKey(begin->GetData());
        }
        else {
          tmpValue.valueType = JsonObject::JsonValueType::STRING_VALUE;
          tmpValue.dataString = begin->GetData();

          if (isArray) {
            jsonObject.GetValue().dataArray.push_back(tmpValue);
          }
          else {
            jsonObject.SetValue(tmpValue);
          }
        }
        break;
      case TokenType::NUMBER_INT:
        tmpValue.valueType = JsonObject::JsonValueType::NUMBER_INT_VALUE;
        tmpValue.dataInt = TokenToInt(*begin);

        if (isArray) {
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        }
        else {
          jsonObject.SetValue(tmpValue);
        }

        break;
      case TokenType::NUMBER_FLOAT:
        tmpValue.valueType = JsonObject::JsonValueType::NUMBER_FLOAT_VALUE;
        tmpValue.dataFloat = TokenToFloat(*begin);

        if (isArray) {
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        }
        else {
          jsonObject.SetValue(tmpValue);
        }

        break;
      case TokenType::BOOL_VALUE:
        tmpValue.valueType = JsonObject::JsonValueType::BOOL_VALUE;
        tmpValue.dataBool = TokenToBool(*begin);

        if (isArray) {
          jsonObject.GetValue().dataArray.push_back(tmpValue);
        }
        else {
          jsonObject.SetValue(tmpValue);
        }

        break;
    }
  }
}

void JsonScanner::ScanToken(std::vector<Token>& tokens, std::vector<char>& fileBuffer) {
  // Fetch the next character in the buffer.
  char c = Advance(fileBuffer);

  // Check which token category it falls into.
  switch (c) {
    case '{':
    {
      Token token(TokenType::OPEN_CURLY_BRACE);
      AddToken(token, tokens);
      break;
    }
    case '}':
    {
      Token token(TokenType::CLOSE_CURLY_BRACE);
      AddToken(token, tokens);
      break;
    }
    case '[':
    {
      Token token(TokenType::OPEN_SQUARE_BRACE);
      AddToken(token, tokens);
      break;
    }
    case ']':
    {
      Token token(TokenType::CLOSE_SQUARE_BRACE);
      AddToken(token, tokens);
      break;
    }
    case ',':
    {
      Token token(TokenType::COMMA);
      AddToken(token, tokens);
      break;
    }
    case ':':
    {
      Token token(TokenType::COLON);
      AddToken(token, tokens);
      break;
    }
    case '"':
      ScanString(tokens, fileBuffer);
      break;
    case '\n':
      mLine++;
      break;
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

void JsonScanner::ScanString(std::vector<Token>& tokens, std::vector<char>& fileBuffer) {
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

  // Add the string to the list of tokens.
  Token token(TokenType::STRING, scannedString);
  AddToken(token, tokens);
}

void JsonScanner::ScanNumber(std::vector<Token>& tokens, std::vector<char>& fileBuffer) {
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

  TokenType tokenType;
  if (realNumber) {
    tokenType = TokenType::NUMBER_FLOAT;
  }
  else {
    tokenType = TokenType::NUMBER_INT;
  }

  std::string scannedNumber(fileBuffer.data() + mStart, fileBuffer.data() + mCurrent);

  Token token(tokenType, scannedNumber);
  AddToken(token, tokens);
}

bool JsonScanner::IsDigit(char letter) {
  return letter >= '0' && letter <= '9';
}

std::int64_t JsonScanner::TokenToInt(Token token) {
  return static_cast<std::int64_t>(std::atoll(token.GetData().c_str()));
}

double JsonScanner::TokenToFloat(Token token) {
  return std::atof(token.GetData().c_str());;
}

bool JsonScanner::TokenToBool(Token token) {
  const std::string& tokenString = token.GetData();

  if (tokenString.compare("true")) {
    return true;
  }
  else if (tokenString.compare("false")) {
    return false;
  }
  else {
    std::cout << "Unknown bool value." << std::endl;
  }

  return false;
}

}
