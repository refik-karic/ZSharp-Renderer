#ifndef ZDRAWING_H
#define ZDRAWING_H

#include <cstddef>

#include "Framebuffer.h"
#include "ZColor.h"

namespace ZSharp {

void DrawRunSlice(Framebuffer& framebuffer, std::size_t x1, std::size_t y1, std::size_t x2, std::size_t y2, ZColor color);

}

#endif
