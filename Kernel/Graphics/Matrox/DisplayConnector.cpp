/*
* Copyright (c) 2022, Kyle Ambroff-Kao <kyle@ambroffkao.com>
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#include <Kernel/Graphics/Matrox/DisplayConnector.h>

namespace Kernel {

bool MatroxDisplayConnector::mutable_mode_setting_capable() const
{
    return false;
}

bool MatroxDisplayConnector::double_framebuffering_capable() const
{
    return false;
}

bool MatroxDisplayConnector::flush_support() const
{
    return false;
}

bool MatroxDisplayConnector::partial_flush_support() const
{
    return false;
}

bool MatroxDisplayConnector::refresh_rate_support() const
{
    return false;
}

ErrorOr<void> MatroxDisplayConnector::set_mode_setting(DisplayConnector::ModeSetting & setting)
{
    return ErrorOr<void>();
}

ErrorOr<void> MatroxDisplayConnector::set_safe_mode_setting()
{
    return ErrorOr<void>();
}

ErrorOr<void> MatroxDisplayConnector::set_y_offset(size_t y)
{
    return ErrorOr<void>();
}

ErrorOr<void> MatroxDisplayConnector::unblank()
{
    return ErrorOr<void>();
}

void MatroxDisplayConnector::enable_console()
{
}

void MatroxDisplayConnector::disable_console()
{
}

ErrorOr<void> MatroxDisplayConnector::flush_first_surface()
{
    return ErrorOr<void>();
}

}
