#include "FileUtilities.h"

#include <fstream>

namespace ZSharp {

  void ReadFileToBuffer(const char* fileName, std::vector<char>& fileBuffer) {
    // Open a stream to the file.
    std::ifstream inputStream(fileName, std::ios::binary);

    // Make sure the file is opened before proceeding.
    if (!inputStream.is_open()) {
      return;
    }

    // Leave it to the standard library to worry about the most efficient platform specific way to read the file.
    fileBuffer = std::vector<char>(std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>());
  }


}
