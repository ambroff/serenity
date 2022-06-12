/*
* Copyright (c) 2022, Kyle Ambroff-Kao <kyle@ambroffkao.com>
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#include <Kernel/Graphics/Matrox/GraphicsAdapter.h>

namespace Kernel {

bool MatroxGraphicsAdapter::vga_compatible() const
{
    return false;
}

}