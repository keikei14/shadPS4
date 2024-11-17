// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <magic_enum.hpp>

#include "SDL3_mixer/SDL_mixer.h"
#include "common/logging/log.h"
#include "core/libraries/audio3d/audio3d.h"
#include "core/libraries/audio3d/audio3d_error.h"
#include "core/libraries/audio3d/audio3d_impl.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/libs.h"

namespace Libraries::Audio3d {

static std::unique_ptr<Audio3dContext> context{};

int PS4_SYSV_ABI sceAudio3dAudioOutClose() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dAudioOutOpen(
    const OrbisAudio3dPortId port_id, const OrbisUserServiceUserId user_id,
    const AudioOut::OrbisAudioOutPort type, const s32 index, const u32 len, const u32 freq,
    const AudioOut::OrbisAudioOutParamExtendedInformation param) {
    if (!context->IsPortValid(port_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    auto port = context->ports[port_id];

    if (len != port.parameters.granularity) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
    }

    const s32 handle = sceAudioOutOpen(user_id, type, index, len, freq, param);
    port.audioout_handle = handle;
    return handle;
}

int PS4_SYSV_ABI sceAudio3dAudioOutOutput(const OrbisAudio3dPortId port_id, const void* ptr) {
    LOG_INFO(Lib_Audio3d, "called, port_id = {}, ptr = {}", port_id, ptr);

    if (!context->IsPortValid(port_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    if (!ptr) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
    }

    return AudioOut::sceAudioOutOutput(context->ports[port_id].audioout_handle, ptr);
}

int PS4_SYSV_ABI sceAudio3dAudioOutOutputs(AudioOut::OrbisAudioOutOutputParam* param,
                                           const u32 num) {
    return sceAudioOutOutputs(param, num);
}

// A bed is a preset channel configuration

int PS4_SYSV_ABI sceAudio3dBedWrite(OrbisAudio3dPortId port_id, u32 num_channels,
                                    OrbisAudio3dFormat format, const uintptr_t buffer,
                                    u32 num_samples) {
    return sceAudio3dBedWrite2(port_id, num_channels, format, buffer, num_samples,
                               ORBIS_AUDIO3D_OUTPUT_BOTH, false);
}

int PS4_SYSV_ABI sceAudio3dBedWrite2(OrbisAudio3dPortId port_id, u32 num_channels,
                                     OrbisAudio3dFormat format, const uintptr_t buffer,
                                     u32 num_samples, OrbisAudio3dOutputRoute output_route,
                                     bool restricted) {
    LOG_INFO(Lib_Audio3d,
             "called, port_id = {}, num_channels = {}, format = {}, num_samples = {}, output_route "
             "= {}, restricted = {}",
             port_id, num_channels, magic_enum::enum_name(format), num_samples,
             magic_enum::enum_name(output_route), restricted);

    if (format <= ORBIS_AUDIO3D_FORMAT_FLOAT &&
        ((num_channels | 4) == 6 || (num_channels | 0x10) == 24) && buffer && num_samples) {
        if (format == ORBIS_AUDIO3D_FORMAT_FLOAT) {
            if ((buffer & 3) != 0) {
                return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
            }
        } else if (format == ORBIS_AUDIO3D_FORMAT_S16) {
            if ((buffer & 1) != 0) {
                return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
            }
        }

        if (output_route > ORBIS_AUDIO3D_OUTPUT_BOTH) {
            return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
        }

        if (!context->IsPortValid(port_id)) {
            return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
        }

        context->queues[port_id].emplace(-1, reinterpret_cast<void*>(buffer));
    }

    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dCreateSpeakerArray() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dDeleteSpeakerArray() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dGetDefaultOpenParameters() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dGetSpeakerArrayMemorySize() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dGetSpeakerArrayMixCoefficients() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dGetSpeakerArrayMixCoefficients2() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dInitialize(const s64 reserved) {
    if (reserved != 0) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
    }

    if (context) {
        return ORBIS_AUDIO3D_ERROR_NOT_READY;
    }

    context = std::make_unique<Audio3dContext>();
    AudioOut::sceAudioOutInit();
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dObjectReserve(const OrbisAudio3dPortId port_id,
                                         OrbisAudio3dObjectId* id) {
    if (!context->IsPortValid(port_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    size_t count = context->objects.size();

    if (count == context->ports[port_id].parameters.max_objects) {
        LOG_ERROR(Lib_Audio3d, "Port {} has reached the maximum number of objects", port_id);
        return ORBIS_AUDIO3D_ERROR_OUT_OF_RESOURCES;
    }

    *id = count + 1;
    context->objects.push_back(Audio3dContext::Object{port_id, *id, {}});
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dObjectSetAttributes(const OrbisAudio3dPortId port_id,
                                               const OrbisAudio3dObjectId object_id,
                                               const size_t num_attributes,
                                               const OrbisAudio3dAttribute* attribute_array) {
    LOG_INFO(Lib_Audio3d, "called, port_id = {}, object_id = {}, num_attributes = {}", port_id,
             object_id, num_attributes);

    if (!context->IsPortValid(port_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    if (!context->IsObjectValid(object_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_OBJECT;
    }

    if (num_attributes == 0 || !attribute_array) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
    }

    for (size_t i = 0; i < num_attributes; i++) {
        const OrbisAudio3dAttribute attribute = attribute_array[i];
        context->objects[object_id].attributes.emplace_back(attribute);
        context->queues[port_id].emplace(object_id, nullptr);
    }

    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dObjectUnreserve(OrbisAudio3dPortId port_id,
                                           OrbisAudio3dObjectId object_id) {
    if (!context->IsPortValid(port_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    if (!context->IsObjectValid(object_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_OBJECT;
    }

    context->DeleteObject(object_id);
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortAdvance(OrbisAudio3dPortId port_id) {
    LOG_INFO(Lib_Audio3d, "called, port_id = {}", port_id);

    if (!context->IsPortValid(port_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    if (context->ports[port_id].parameters.buffer_mode == ORBIS_AUDIO3D_BUFFER_NO_ADVANCE) {
        LOG_ERROR(Lib_Audio3d, "Port {} has no advance capability", port_id);
        return ORBIS_AUDIO3D_ERROR_NOT_SUPPORTED;
    }

    if (context->queues[port_id].empty()) {
        LOG_ERROR(Lib_Audio3d, "Queue is empty, cannot pop port {}", port_id);
        return ORBIS_OK;
    }

    context->queues[port_id].pop();
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortClose(const OrbisAudio3dPortId port_id) {
    if (port_id > context->ports.size()) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    context->DeletePort(port_id);
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortCreate() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortDestroy() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortFlush() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortFreeState() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortGetAttributesSupported() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortGetList() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortGetParameters() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortGetQueueLevel(OrbisAudio3dPortId port_id, u32* queue_level,
                                             u32* queue_available) {
    LOG_INFO(Lib_Audio3d, "called, port_id = {}", port_id);

    if (!context->IsPortValid(port_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    if (!queue_level && !queue_available) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
    }

    auto size = context->queues[port_id].size();
    auto parameters = context->ports[port_id].parameters;

    if (queue_level) {
        *queue_level = size;
    }

    if (queue_available) {
        u32 queue_depth = parameters.queue_depth;
        u32 available = parameters.queue_depth - context->queues[port_id].size();
        LOG_INFO(Lib_Audio3d, "queue_depth = {}, size = {}", queue_depth, size);
        *queue_available = available;
    }

    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortGetState() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortGetStatus() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortOpen(OrbisUserServiceUserId user_id,
                                    const OrbisAudio3dOpenParameters* parameters,
                                    OrbisAudio3dPortId* id) {
    if (!parameters || parameters->rate != 0 ||
        parameters->buffer_mode > ORBIS_AUDIO3D_BUFFER_ADVANCE_AND_PUSH ||
        (parameters->granularity < 256 || parameters->max_objects == 0 ||
         parameters->queue_depth == 0)) {
        LOG_ERROR(Lib_Audio3d, "Invalid parameters");
        return ORBIS_AUDIO3D_ERROR_INVALID_PARAMETER;
    }

    if (!context) {
        return ORBIS_AUDIO3D_ERROR_NOT_READY;
    }

    *id = context->ports.size() + 1;
    // Make a copy to avoid dangling references
    OrbisAudio3dOpenParameters parameters_copy = *parameters;
    context->ports.resize(*id + 1);
    context->ports[*id] = {*id, parameters_copy, -1};
    context->queues.resize(*id + 1);
    context->queues[*id] = {};
    LOG_INFO(Lib_Audio3d, "size: {}", context->ports.size());
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortPush(OrbisAudio3dPortId port_id, SceAudio3dBlocking blocking) {
    LOG_INFO(Lib_Audio3d, "called, port_id = {}, blocking = {}", port_id,
             magic_enum::enum_name(blocking));

    if (!context->IsPortValid(port_id)) {
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    auto port = context->ports[port_id];

    if (port.parameters.buffer_mode != ORBIS_AUDIO3D_BUFFER_ADVANCE_AND_PUSH) {
        LOG_ERROR(Lib_Audio3d, "Port {} has no push capability", port_id);
        return ORBIS_AUDIO3D_ERROR_NOT_SUPPORTED;
    }

    auto queue = context->queues[port_id];

    if (queue.empty()) {
        LOG_ERROR(Lib_Audio3d, "Queue is empty, cannot push port {}", port_id);
        return ORBIS_OK;
    }

    auto front = queue.front();

    // No object, but there's a PCM buffer from BedWrite2
    if (front.first == u32(-1) && front.second) {
        AudioOut::sceAudioOutOutput(1, front.second);
        queue.pop();
        return ORBIS_OK;
    } else if (!front.first && !front.second) {
        return ORBIS_OK;
    }

    auto object = context->objects[front.first];

    for (const auto attribute : object.attributes) {
        if (AttributeId(attribute.attribute_id) != PCM) {
            continue;
        }

        AudioOut::sceAudioOutOutput(1, attribute.value);
    }

    queue.pop();

    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortQueryDebug() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dPortSetAttribute() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dReportRegisterHandler() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dReportUnregisterHandler() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dSetGpuRenderer() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dStrError() {
    LOG_ERROR(Lib_Audio3d, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceAudio3dTerminate() {
    if (!context) {
        return ORBIS_AUDIO3D_ERROR_NOT_READY;
    }

    LOG_INFO(Lib_Audio3d, "term");
    return ORBIS_OK;
}

void RegisterlibSceAudio3d(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("pZlOm1aF3aA", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dAudioOutClose);
    LIB_FUNCTION("ucEsi62soTo", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dAudioOutOpen);
    LIB_FUNCTION("7NYEzJ9SJbM", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dAudioOutOutput);
    LIB_FUNCTION("HbxYY27lK6E", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dAudioOutOutputs);
    LIB_FUNCTION("9tEwE0GV0qo", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dBedWrite);
    LIB_FUNCTION("xH4Q9UILL3o", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dBedWrite2);
    LIB_FUNCTION("lvWMW6vEqFU", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dCreateSpeakerArray);
    LIB_FUNCTION("8hm6YdoQgwg", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dDeleteSpeakerArray);
    LIB_FUNCTION("Im+jOoa5WAI", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dGetDefaultOpenParameters);
    LIB_FUNCTION("kEqqyDkmgdI", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dGetSpeakerArrayMemorySize);
    LIB_FUNCTION("-R1DukFq7Dk", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dGetSpeakerArrayMixCoefficients);
    LIB_FUNCTION("-Re+pCWvwjQ", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dGetSpeakerArrayMixCoefficients2);
    LIB_FUNCTION("UmCvjSmuZIw", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dInitialize);
    LIB_FUNCTION("jO2tec4dJ2M", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dObjectReserve);
    LIB_FUNCTION("4uyHN9q4ZeU", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dObjectSetAttributes);
    LIB_FUNCTION("1HXxo-+1qCw", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dObjectUnreserve);
    LIB_FUNCTION("lw0qrdSjZt8", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortAdvance);
    LIB_FUNCTION("OyVqOeVNtSk", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortClose);
    LIB_FUNCTION("UHFOgVNz0kk", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortCreate);
    LIB_FUNCTION("Mw9mRQtWepY", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortDestroy);
    LIB_FUNCTION("ZOGrxWLgQzE", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortFlush);
    LIB_FUNCTION("uJ0VhGcxCTQ", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortFreeState);
    LIB_FUNCTION("9ZA23Ia46Po", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dPortGetAttributesSupported);
    LIB_FUNCTION("SEggctIeTcI", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortGetList);
    LIB_FUNCTION("flPcUaXVXcw", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dPortGetParameters);
    LIB_FUNCTION("YaaDbDwKpFM", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dPortGetQueueLevel);
    LIB_FUNCTION("CKHlRW2E9dA", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortGetState);
    LIB_FUNCTION("iRX6GJs9tvE", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortGetStatus);
    LIB_FUNCTION("XeDDK0xJWQA", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortOpen);
    LIB_FUNCTION("VEVhZ9qd4ZY", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dPortPush);
    LIB_FUNCTION("-pzYDZozm+M", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dPortQueryDebug);
    LIB_FUNCTION("Yq9bfUQ0uJg", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dPortSetAttribute);
    LIB_FUNCTION("QfNXBrKZeI0", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dReportRegisterHandler);
    LIB_FUNCTION("psv2gbihC1A", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dReportUnregisterHandler);
    LIB_FUNCTION("yEYXcbAGK14", "libSceAudio3d", 1, "libSceAudio3d", 1, 1,
                 sceAudio3dSetGpuRenderer);
    LIB_FUNCTION("Aacl5qkRU6U", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dStrError);
    LIB_FUNCTION("WW1TS2iz5yc", "libSceAudio3d", 1, "libSceAudio3d", 1, 1, sceAudio3dTerminate);
};

} // namespace Libraries::Audio3d