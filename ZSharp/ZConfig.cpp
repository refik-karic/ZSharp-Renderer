#include "ZConfig.h"

namespace ZSharp {

ZConfig& ZConfig::GetInstance() {
  static ZConfig singleton;
  return singleton;
}

}
