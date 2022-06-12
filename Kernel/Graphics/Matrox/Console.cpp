/*
* Copyright (c) 2022, Kyle Ambroff-Kao <kyle@ambroffkao.com>
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#include <Kernel/Graphics/Matrox/Console.h>

namespace Kernel {

void MatroxFramebufferConsole::set_resolution(size_t width, size_t height, size_t pitch)
{
}

u8* MatroxFramebufferConsole::framebuffer_data()
{
    return nullptr;
}

void MatroxFramebufferConsole::flush(size_t x, size_t y, size_t width, size_t height)
{
}

}
