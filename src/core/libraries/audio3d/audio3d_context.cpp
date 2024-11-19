// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/logging/log.h"
#include "core/libraries/audio3d/audio3d_context.h"
#include "core/libraries/audio3d/audio3d_error.h"

namespace Libraries::Audio3d {

u32 Audio3dContext::PortOpen(OrbisAudio3dOpenParameters parameters) {
    // Create a copy to avoid dangling references
    auto parameters_copy = parameters;

    u32 id = m_ports.size() + 1;
    m_ports[id] = {parameters_copy, std::nullopt};
    return id;
}

u32 Audio3dContext::CreateQueueObject(u32 port_id,
                                      const std::vector<OrbisAudio3dAttribute>& attributes,
                                      void* pcm) {
    if (m_ports.find(port_id) == m_ports.end()) {
        LOG_DEBUG(Lib_Audio3d, "invalid port");
        return ORBIS_AUDIO3D_ERROR_INVALID_PORT;
    }

    if (pcm) {
        LOG_DEBUG(Lib_Audio3d, "pcm");
        m_queues[port_id].emplace(std::nullopt, std::nullopt, pcm);
        return 0;
    }

    LOG_DEBUG(Lib_Audio3d, "no pcm");
    u32 id = m_queues[port_id].size() + 1;
    m_queues[port_id].emplace(id, attributes, nullptr);
    return id;
}

} // namespace Libraries::Audio3d
