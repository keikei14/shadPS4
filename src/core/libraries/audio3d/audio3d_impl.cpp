// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "audio3d_error.h"
#include "audio3d_impl.h"

#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/kernel/libkernel.h"

using namespace Libraries::Kernel;

namespace Libraries::Audio3d {

bool Audio3dContext::IsPortValid(const OrbisAudio3dPortId port_id) const {
    return port_id <= ports.size();
}

bool Audio3dContext::IsObjectValid(const OrbisAudio3dObjectId object_id) const {
    return object_id <= ports.size();
}

} // namespace Libraries::Audio3d
