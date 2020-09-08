#pragma once

#include <vector>

namespace ZSharp {
  /// <summary>
  /// Read the entire file into memory and store it in the buffer.
  /// </summary>
  /// <param name='fileName'>Name of the file to open.</param>
  /// <param name='buffer'>The buffer to hold the contents of the file in memory.</param>
  /// <returns>True if successfully read from the file, False otherwise.</returns>
  void ReadFileToBuffer(const char* fileName, std::vector<char>& fileBuffer);


}
