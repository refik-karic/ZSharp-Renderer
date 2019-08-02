#include "GlobalEdgeTable.h"

namespace ZSharp {

void GlobalEdgeTable::AddPoint(std::size_t yIndex, std::size_t x, ZColor color, std::size_t primitiveIndex) {
  ScanLineList& list = mEdgeTable[yIndex];

  if (list.empty() || primitiveIndex != mLastActivePrimitive) {
    ScanLine scanLine{
      x,
      x,
      color
    };

    list.push_back(scanLine);
    mLastActivePrimitive = primitiveIndex;
  }
  else {
    ScanLine& scanLine = list[list.size() - 1];

    if (scanLine.x1 > x) {
      scanLine.x1 = x;
    }
    else if (scanLine.x2 < x) {
      scanLine.x2 = x;
    }

    scanLine.color = color;
  }
}

void GlobalEdgeTable::Clear() {
  mEdgeTable.clear();
  mLastActivePrimitive = 0;
}

void GlobalEdgeTable::Draw(Framebuffer& frameBuffer) {
  for (auto& [y, scanLineList] : mEdgeTable) {
    for (auto& scanLine : scanLineList) {
      frameBuffer.SetRow(y, scanLine.x1, scanLine.x2, scanLine.color);
    }
  }
}

}
