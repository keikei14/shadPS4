// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <vector>

#include "common/types.h"

namespace Libraries::Audio3d {

using OrbisAudio3dPortId = u32;
using OrbisAudio3dObjectId = u32;
using OrbisAudio3dAttributeId = u32;

enum OrbisAudio3dRate { ORBIS_AUDIO3D_RATE_48000 = 0 };

enum OrbisAudio3dBufferMode {
    ORBIS_AUDIO3D_BUFFER_NO_ADVANCE = 0,
    ORBIS_AUDIO3D_BUFFER_ADVANCE_NO_PUSH = 1,
    ORBIS_AUDIO3D_BUFFER_ADVANCE_AND_PUSH = 2
};

enum OrbisAudio3dFormat {
    ORBIS_AUDIO3D_FORMAT_S16 = 0x0,
    ORBIS_AUDIO3D_FORMAT_FLOAT = 0x1,
};

enum OrbisAudio3dOutputRoute {
    ORBIS_AUDIO3D_OUTPUT_BOTH = 0x0,
    ORBIS_AUDIO3D_OUTPUT_HMU_ONLY = 0x1,
    ORBIS_AUDIO3D_OUTPUT_TV_ONLY = 0x2,
};

struct OrbisAudio3dOpenParameters {
    size_t size_this;
    u32 granularity;
    OrbisAudio3dRate rate;
    u32 max_objects;
    u32 queue_depth;
    OrbisAudio3dBufferMode buffer_mode;
    int : 32; // Padding
    u32 num_beds;
};

enum AttributeId : u32 {
    PCM = 0x1,
    PRIORITY,
    POSITION,
    SPREAD,
    GAIN,
    PASSTHROUGH,
    RESET_STATE,
    APPLICATION_SPECIFIC,
    AMBISONICS,
    RESTRICTED,
    OUTPUT_ROUTE,
    LATE_REVERB_LEVEL = 0x10001,
    DOWNMIX_SPREAD_RADIUS,
    DOWNMIX_SPREAD_HEIGHT_AWARE,
};

struct OrbisAudio3dAttribute {
    AttributeId attribute_id;
    int : 32; // Padding
    const void* value;
    size_t value_size;
};

class Audio3dContext {
public:
    Audio3dContext() = default;
    ~Audio3dContext() = default;

    [[nodiscard]] bool IsPortValid(OrbisAudio3dPortId port_id) const;
    [[nodiscard]] bool IsObjectValid(OrbisAudio3dObjectId object_id) const;

    std::vector<std::tuple<OrbisAudio3dPortId, OrbisAudio3dOpenParameters, s32>> ports{};
    std::vector<OrbisAudio3dObjectId> objects{};
    std::vector<std::pair<OrbisAudio3dAttributeId, OrbisAudio3dAttribute>> attributes{};
};

} // namespace Libraries::Audio3d
