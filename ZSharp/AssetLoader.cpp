#include <fstream>

#include "AssetLoader.h"
#include "Model.h"

namespace ZSharp {

template <typename T>
static Model<T> AssetLoader::ParseFile(const std::string& fileName) {
  // Open a stream to the file.
  std::ifstream inputStream(fileName, std::ios::binary);

  // Make sure the file is opened before proceeding.
  if (!inputStream.is_open()) {
    return nullptr;
  }

  // Only UTF-8 files are supported.
  inputStream.imbue();

  // Read the file in blocks and send them for processing.
  char buffer[BLOCK_SIZE];
  uint32_t bytesRead(0);

  do {

  } while (bytesRead > 0);
}

}
