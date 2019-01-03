#ifndef ZDRAWING_H
#define ZDRAWING_H

#include <cstdint>
#include <cstddef>

#include "Framebuffer.h"
#include "ZColor.h"

namespace ZSharp {

void DrawRunSlice(Framebuffer& framebuffer, std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, ZColor color);

}

#endif
