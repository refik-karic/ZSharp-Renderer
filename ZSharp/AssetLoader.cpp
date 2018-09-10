#include "AssetLoader.h"
#include "Model.h"

namespace ZSharp {

uint32_t AssetLoader::ParseTokens(uint32_t index, uint32_t bytesRemaining, std::list<JsonValuePair>& tokens) {
  uint32_t bytesProcessed(0);
  uint32_t lookAhead(0);

  // TODO: Fix a bug where some tokens get thrown out for some reason.
  // Has something to do with the way I'm indexing past default tokens.
  // Need a better way to throw out invalid tokens.

  while (bytesProcessed < bytesRemaining) {
    JsonValuePair pair;

    switch (mData[(index + bytesProcessed) % mBlockSize]) {
      case static_cast<uint8_t>(' ') :
      case static_cast<uint8_t>('\r') :
      case static_cast<uint8_t>('\n') :
        bytesProcessed++;
        break;
      case static_cast<uint8_t>('{') :
        pair.token = JsonTokens::OPEN_CURLY_BRACE;
        tokens.push_back(pair);
        bytesProcessed++;
        break;
      case static_cast<uint8_t>('}') :
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

        // Get the look ahead index for the ending symbol/end of buffer.
        lookAhead = LookAhead(static_cast<uint8_t>('"'), index, bytesProcessed + 1, bytesRemaining);

        // Check once more if the look ahead position is the end of token.
        if (mData[(index + lookAhead) % mBlockSize] != static_cast<uint8_t>('"')) {
          // Shift the remaining data to the end of the buffer so that it doesn't get overriden by the next read() call.
          ShiftData(index + bytesProcessed, bytesRemaining - bytesProcessed);

          // Read another chunk of data to try and find the end of this token.
          return bytesRemaining - bytesProcessed;
        }

        // Create a string object based off of a substring of the literal token.
        {
          std::string temp(lookAhead - bytesProcessed + 1, '\0');

          // Unfortunetly a single memcpy() can't be used because this is a circular buffer.
          // TODO: Look into using two memcpy()'s by calling one from the current index to the end and the other
          // when it wraps around to the final index.
          for (uint32_t dataIndex(bytesProcessed), tempIndex(0); dataIndex < lookAhead; dataIndex++, tempIndex++) {
            temp[tempIndex] = mData[dataIndex % mBlockSize];
          }

          pair.dataString = temp;
        }

        tokens.push_back(pair);
        // Add the delta from the look ahead.
        bytesProcessed = bytesProcessed + (lookAhead - bytesProcessed) + 1;
        break;
      default:
        // Unknown token was discovered.
        // Look ahead until the next whitespace and try to decide if it is a value type of:
        // 1. Number
        // 2. Boolean
        lookAhead = LookAhead(static_cast<uint8_t>(' '), index, bytesProcessed + 1, bytesRemaining);

        // Check once more if the look ahead position is the end of the token.
        if (mData[(index + lookAhead) % mBlockSize] != static_cast<uint8_t>(' ')) {
          // Shift the remaining data to the end of the buffer so that it doesn't get overriden by the next read() call.
          ShiftData(index + bytesProcessed, bytesRemaining - bytesProcessed);

          // Read another chunk of data to try and find the end of this token.
          return bytesRemaining - bytesProcessed;
        }

        // Parse out the value from this token.
        {
          std::string temp(lookAhead - bytesProcessed + 1, '\0');
          // Unfortunetly a single memcpy() can't be used because this is a circular buffer.
          // TODO: Look into using two memcpy()'s by calling one from the current index to the end and the other
          // when it wraps around to the final index.
          for (uint32_t dataIndex(bytesProcessed), tempIndex(0); dataIndex < lookAhead; dataIndex++, tempIndex++) {
            temp[tempIndex] = mData[dataIndex % mBlockSize];
          }

          // Check if this is a floating point number.
          if (temp.find(".") != std::string::npos) {
            // Parse a floating point number from this string.
            pair.token = JsonTokens::NUMBER_FLOAT;
            pair.dataFloat = std::stod(temp);
          }
          else if(temp.find("true") != std::string::npos) {
            // Parse a boolean value from the string.
            pair.token = JsonTokens::BOOLEAN;
            pair.dataBool = true;
          }
          else if (temp.find("false") != std::string::npos) {
            // Parse a boolean value from the string.
            pair.token = JsonTokens::BOOLEAN;
            pair.dataBool = false;
          }
          else {
            // Parse an integer number from the string.
            pair.token = JsonTokens::NUMBER_INT;
            pair.dataInt = static_cast<int64_t>(atol(temp.c_str()));
          }
        }

        // Add the value token to the list and continue parsing.
        tokens.push_back(pair);
        bytesProcessed = bytesProcessed + (lookAhead - bytesProcessed) + 1;
    }
  }

  // This should only ever happen if something went wrong.
  // In that case, any data in the existing buffer will be overwritten on the next file read.
  return 0;
}

uint32_t AssetLoader::LookAhead(uint8_t symbol, uint32_t bufferIndex, uint32_t lookIndex, uint32_t end) {
  while ((lookIndex < end) &&
    (mData[(bufferIndex + lookIndex) % mBlockSize] != symbol)) {
    lookIndex++;
  }

  return lookIndex;
}

void AssetLoader::ShiftData(uint32_t index, uint32_t bytesRemaining) {
  while (bytesRemaining > 0) {
    mData[mBlockSize - bytesRemaining] = mData[index % mBlockSize];
    bytesRemaining--;
    index++;
  }
}
}
