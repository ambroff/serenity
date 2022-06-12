/*
* Copyright (c) 2022, Kyle Ambroff-Kao <kyle@ambroffkao.com>
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#include <Kernel/Graphics/DisplayConnector.h>

namespace Kernel {

class MatroxDisplayConnector final : DisplayConnector {
    bool mutable_mode_setting_capable() const override;
    bool double_framebuffering_capable() const override;
    bool flush_support() const override;
    bool partial_flush_support() const override;
    bool refresh_rate_support() const override;
    ErrorOr<void> set_mode_setting(ModeSetting & setting) override;
    ErrorOr<void> set_safe_mode_setting() override;
    ErrorOr<void> set_y_offset(size_t y) override;
    ErrorOr<void> unblank() override;
    void enable_console() override;
    void disable_console() override;
    ErrorOr<void> flush_first_surface() override;
};

}
