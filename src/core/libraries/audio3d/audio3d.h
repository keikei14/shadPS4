// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"
#include "core/libraries/audio/audioout.h"
#include "core/libraries/audio3d/audio3d_impl.h"

namespace Core::Loader {
class SymbolsResolver;
}

namespace Libraries::Audio3d {

using OrbisUserServiceUserId = s32;

int PS4_SYSV_ABI sceAudio3dAudioOutClose();
int PS4_SYSV_ABI sceAudio3dAudioOutOpen(OrbisAudio3dPortId port_id, OrbisUserServiceUserId user_id,
                                        AudioOut::OrbisAudioOutPort type, s32 index, u32 len,
                                        u32 freq,
                                        AudioOut::OrbisAudioOutParamExtendedInformation param);
int PS4_SYSV_ABI sceAudio3dAudioOutOutput(OrbisAudio3dPortId port_id, const void* ptr);
int PS4_SYSV_ABI sceAudio3dAudioOutOutputs(AudioOut::OrbisAudioOutOutputParam* param, u32 num);
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
int PS4_SYSV_ABI sceAudio3dInitialize(s64 reserved);
int PS4_SYSV_ABI sceAudio3dObjectReserve(OrbisAudio3dPortId port_id, OrbisAudio3dObjectId* id);
int PS4_SYSV_ABI sceAudio3dObjectSetAttributes(OrbisAudio3dPortId port_id,
                                               OrbisAudio3dObjectId object_id,
                                               size_t num_attributes,
                                               const OrbisAudio3dAttribute* attribute_array);
int PS4_SYSV_ABI sceAudio3dObjectUnreserve();
int PS4_SYSV_ABI sceAudio3dPortAdvance();
int PS4_SYSV_ABI sceAudio3dPortClose(OrbisAudio3dPortId id);
int PS4_SYSV_ABI sceAudio3dPortCreate();
int PS4_SYSV_ABI sceAudio3dPortDestroy();
int PS4_SYSV_ABI sceAudio3dPortFlush();
int PS4_SYSV_ABI sceAudio3dPortFreeState();
int PS4_SYSV_ABI sceAudio3dPortGetAttributesSupported();
int PS4_SYSV_ABI sceAudio3dPortGetList();
int PS4_SYSV_ABI sceAudio3dPortGetParameters();
int PS4_SYSV_ABI sceAudio3dPortGetQueueLevel(OrbisAudio3dPortId port_id, u32* queue_level,
                                             u32* queue_available);
int PS4_SYSV_ABI sceAudio3dPortGetState();
int PS4_SYSV_ABI sceAudio3dPortGetStatus();
int PS4_SYSV_ABI sceAudio3dPortOpen(OrbisUserServiceUserId user_id,
                                    const OrbisAudio3dOpenParameters* parameters,
                                    OrbisAudio3dPortId* id);
int PS4_SYSV_ABI sceAudio3dPortPush(OrbisAudio3dPortId port_id, SceAudio3dBlocking blocking);
int PS4_SYSV_ABI sceAudio3dPortQueryDebug();
int PS4_SYSV_ABI sceAudio3dPortSetAttribute();
int PS4_SYSV_ABI sceAudio3dReportRegisterHandler();
int PS4_SYSV_ABI sceAudio3dReportUnregisterHandler();
int PS4_SYSV_ABI sceAudio3dSetGpuRenderer();
int PS4_SYSV_ABI sceAudio3dStrError();
int PS4_SYSV_ABI sceAudio3dTerminate();

void RegisterlibSceAudio3d(Core::Loader::SymbolsResolver* sym);
} // namespace Libraries::Audio3d