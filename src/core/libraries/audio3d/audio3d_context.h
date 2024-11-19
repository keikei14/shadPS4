// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "core/libraries/audio3d/audio3d.h"

#include <optional>
#include <unordered_map>
#include <queue>

namespace Libraries::Audio3d {

class Audio3dContext {
public:
    Audio3dContext() = default;
    ~Audio3dContext() = default;

    u32 PortOpen(OrbisAudio3dOpenParameters parameters);
    u32 CreateQueueObject(u32 port_id, const std::vector<OrbisAudio3dAttribute>& attributes,
                          void* pcm);

private:
    struct Port {
        OrbisAudio3dOpenParameters parameters{};
        std::optional<s32> audio_out_handle;
    };

    struct QueueObject {
        // These are optional as it may only be a PCM buffer from BedWrite/BedWrite2
        // TODO: There might be a better way to handle this
        std::optional<OrbisAudio3dObjectId> object_id;
        std::optional<std::vector<OrbisAudio3dAttribute>> attributes;
        void* pcm{};
    };

    std::unordered_map<u32, Port> m_ports;
    std::unordered_map<u32, std::queue<QueueObject>> m_queues;
};

} // namespace Libraries::Audio3d
