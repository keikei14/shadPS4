// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"

namespace Core::Loader {
class SymbolsResolver;
}

namespace Libraries::Audio3d {

using OrbisUserServiceUserId = s32;
using OrbisAudio3dPortId = u32;
using OrbisAudio3dObjectId = u32;

enum OrbisAudio3dRate { ORBIS_AUDIO3D_RATE_48000 = 0 };

enum OrbisAudio3dBufferMode {
    ORBIS_AUDIO3D_BUFFER_NO_ADVANCE = 0,
    ORBIS_AUDIO3D_BUFFER_ADVANCE_NO_PUSH,
    ORBIS_AUDIO3D_BUFFER_ADVANCE_AND_PUSH,
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

static_assert(sizeof(OrbisAudio3dOpenParameters) == 0x28);

struct OrbisAudio3dAttribute {
    u32 attribute_id;
    int : 32; // Padding
    const void* value;
    size_t value_size;
};

static_assert(sizeof(OrbisAudio3dAttribute) == 0x18);

int PS4_SYSV_ABI sceAudio3dAudioOutClose();
int PS4_SYSV_ABI sceAudio3dAudioOutOpen();
int PS4_SYSV_ABI sceAudio3dAudioOutOutput();
int PS4_SYSV_ABI sceAudio3dAudioOutOutputs();
int PS4_SYSV_ABI sceAudio3dBedWrite(OrbisAudio3dPortId port_id, u32 num_channels,
                                    OrbisAudio3dFormat format, uintptr_t buffer, u32 num_samples);
int PS4_SYSV_ABI sceAudio3dBedWrite2(OrbisAudio3dPortId port_id, u32 num_channels,
                                     OrbisAudio3dFormat format, uintptr_t buffer, u32 num_samples,
                                     OrbisAudio3dOutputRoute output_route, bool restricted);
int PS4_SYSV_ABI sceAudio3dCreateSpeakerArray();
int PS4_SYSV_ABI sceAudio3dDeleteSpeakerArray();
int PS4_SYSV_ABI sceAudio3dGetDefaultOpenParameters();
int PS4_SYSV_ABI sceAudio3dGetSpeakerArrayMemorySize();
int PS4_SYSV_ABI sceAudio3dGetSpeakerArrayMixCoefficients();
int PS4_SYSV_ABI sceAudio3dGetSpeakerArrayMixCoefficients2();
int PS4_SYSV_ABI sceAudio3dInitialize(const s64 reserved);
int PS4_SYSV_ABI sceAudio3dObjectReserve();
int PS4_SYSV_ABI sceAudio3dObjectSetAttributes();
int PS4_SYSV_ABI sceAudio3dObjectUnreserve();
int PS4_SYSV_ABI sceAudio3dPortAdvance();
int PS4_SYSV_ABI sceAudio3dPortClose();
int PS4_SYSV_ABI sceAudio3dPortCreate();
int PS4_SYSV_ABI sceAudio3dPortDestroy();
int PS4_SYSV_ABI sceAudio3dPortFlush();
int PS4_SYSV_ABI sceAudio3dPortFreeState();
int PS4_SYSV_ABI sceAudio3dPortGetAttributesSupported();
int PS4_SYSV_ABI sceAudio3dPortGetList();
int PS4_SYSV_ABI sceAudio3dPortGetParameters();
int PS4_SYSV_ABI sceAudio3dPortGetQueueLevel();
int PS4_SYSV_ABI sceAudio3dPortGetState();
int PS4_SYSV_ABI sceAudio3dPortGetStatus();
int PS4_SYSV_ABI sceAudio3dPortOpen(OrbisUserServiceUserId user_id,
                                    const OrbisAudio3dOpenParameters* parameters,
                                    OrbisAudio3dPortId* id);
int PS4_SYSV_ABI sceAudio3dPortPush();
int PS4_SYSV_ABI sceAudio3dPortQueryDebug();
int PS4_SYSV_ABI sceAudio3dPortSetAttribute();
int PS4_SYSV_ABI sceAudio3dReportRegisterHandler();
int PS4_SYSV_ABI sceAudio3dReportUnregisterHandler();
int PS4_SYSV_ABI sceAudio3dSetGpuRenderer();
int PS4_SYSV_ABI sceAudio3dStrError();
int PS4_SYSV_ABI sceAudio3dTerminate();

void RegisterlibSceAudio3d(Core::Loader::SymbolsResolver* sym);
} // namespace Libraries::Audio3d