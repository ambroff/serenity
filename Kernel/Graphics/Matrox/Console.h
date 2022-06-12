/*
 * Copyright (c) 2022, Kyle Ambroff-Kao <kyle@ambroffkao.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Kernel/Graphics/Console/GenericFramebufferConsole.h>

namespace Kernel {

class MatroxFramebufferConsole final : public Graphics::GenericFramebufferConsole {
public:
    void set_resolution(size_t width, size_t height, size_t pitch) override;
    void flush(size_t x, size_t y, size_t width, size_t height) override;

protected:
    u8* framebuffer_data() override;
};

}
