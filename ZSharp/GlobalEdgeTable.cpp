#include "GlobalEdgeTable.h"

namespace ZSharp {

void GlobalEdgeTable::AddPoint(std::size_t yIndex, std::size_t x, ZColor color) {
  ScanLineList& list = mEdgeTable[yIndex];

  if (list.empty() || mParity) {
    ScanLine scanLine{
      x,
      0,
      color
    };

    list.push_back(scanLine);
    mParity = false;
  }
  else {
    std::size_t end = list.size() - 1;
    ScanLine scanLine = list[end];
    /*
    if (scanLine.x1 == x || scanLine.x2 == x) {
      list.pop_back();
      return;
    }
    */
    if (scanLine.x1 < x) {
      scanLine.x2 = x;
    }
    else {
      scanLine.x2 = scanLine.x1;
      scanLine.x1 = x;
    }

    scanLine.color = color;
    list[end] = scanLine;
    mParity = true;
  }
}

void GlobalEdgeTable::Clear() {
  mEdgeTable.clear();
}

void GlobalEdgeTable::Draw(Framebuffer& frameBuffer) {
  for (auto& [y, scanLineList] : mEdgeTable) {
    for (auto& scanLine : scanLineList) {
      frameBuffer.SetRow(y, scanLine.x1, scanLine.x2, scanLine.color);
    }
  }
}

}
