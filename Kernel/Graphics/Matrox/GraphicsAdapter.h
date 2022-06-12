/*
 * Copyright (c) 2022, Kyle Ambroff-Kao <kyle@ambroffkao.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <Kernel/Bus/PCI/Device.h>
#include <Kernel/Graphics/GenericGraphicsAdapter.h>

namespace Kernel {

class MatroxGraphicsAdapter
    : public GenericGraphicsAdapter
    , public PCI::Device {

public:
    bool vga_compatible() const override;
};

}
