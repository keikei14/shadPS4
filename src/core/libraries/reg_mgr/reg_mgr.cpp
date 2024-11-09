// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/assert.h"
#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/libs.h"
#include "core/libraries/reg_mgr/ent_keys.h"
#include "core/libraries/reg_mgr/reg_mgr.h"

namespace Libraries::RegMgr {

// Taken from fpPS4

const u8 s_ScrambNSsdk[] = {0x6B, 0xE8, 0x98, 0x03, 0x9A, 0x70, 0x23, 0x5A,
                            0x63, 0xEE, 0xF5, 0x7B, 0xFF, 0xA4, 0x4C, 0x8C};

const u8 s_ScrambNSlib[] = {0x14, 0xEE, 0xDE, 0xE1, 0x80, 0xAC, 0xF3, 0x78,
                            0x47, 0x43, 0xDB, 0x40, 0x93, 0xDD, 0xB1, 0x34};

// Also taken from fpPS4
static u32 decode(u64 in) {
    u8 a = in & 0xFF;            // data[0]
    u8 b = (in >> 8) & 0xFF;     // data[1]
    u8 c = (in >> 16) & 0xFF;    // data[2]
    u8 d = (in >> 24) & 0xFF;    // data[3]
    u8 e = (in >> 32) & 0xFF;    // table
    u8 f = (in >> 40) & 0xFF;    // index
    u16 g = (in >> 48) & 0xFFFF; // checksum
    u16 h = (a + b + c + d + e * f) & 0xFFFF;

    if (g != h) {
        return 0x800D0204;
    }

    f = f ^ 0x6B; // Bitwise XOR

    if (f > 12) {
        return 0x800D0203;
    }

    auto x = e ^ s_ScrambNSsdk[0xF - f];
    const u8* p_Scramb;

    if (x == 0x19) {
        p_Scramb = s_ScrambNSlib;
    } else {
        p_Scramb = s_ScrambNSsdk;
        if (x != 0x72) {
            return 0x800D0205;
        }
    }

    x = p_Scramb[f];
    u8 y = p_Scramb[f + 1];

    u8 data[4] = {
        u8(a ^ p_Scramb[f + 3]),
        u8(c ^ p_Scramb[f + 2]),
        u8(d ^ y),
        u8(b ^ x),
    };

    return (data[0]) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
}

int PS4_SYSV_ABI sceRegMgrBackupNeedMem() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrBackupPullData() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrBackupPushData() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrCheckError() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrCntlDeleteReg() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrCntlGetFileName() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrCntlGetFilesCount() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrCntlStart() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrCntlStopHdd() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrCntSetInitReinst() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrDrvDataCheckGet() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrDrvDataClose() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrDrvDataOpen() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrDrvGetEntCnt() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrDrvGetUpdateCnt() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrEvtGetCnt() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrEvtGetRegId() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

/*
 * Some sort of map with all possible keys would be nice. But not many keys are required, so this
 * method is okay.
 */
int PS4_SYSV_ABI sceRegMgrGetBin(u32 key, void* out, size_t out_size) {
    LOG_INFO(Lib_RegMgr, "called, key = {}, out = {}, out_size = {}", key, static_cast<void*>(out),
             out_size);

    switch (key) {
        // TODO
    default:
        LOG_ERROR(Lib_RegMgr, "Unknown key {}", key);
    }

    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrGetBinInitVal() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrGetInt(u32 key, int* out) {
    LOG_INFO(Lib_RegMgr, "called, key = {}, out = {}", key, static_cast<void*>(out));

    switch (key) {
    case SCE_REGMGR_ENT_KEY_NET_APP_ssl_cert_ignore:
        return 1;
    default:
        LOG_ERROR(Lib_RegMgr, "Unknown key {}", key);
    }

    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrGetIntInitVal() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrGetStr(u32 key, char* out, size_t out_size) {
    LOG_INFO(Lib_RegMgr, "called, key = {}, out = {}, out_size = {}", key, static_cast<void*>(out),
             out_size);

    switch (key) {
        // TODO
    default:
        LOG_ERROR(Lib_RegMgr, "Unknown key {}", key);
    }

    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrGetStrInitVal() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrGetVersion() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrIsChange() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrIsInitOK() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrLogPull() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrLogStart() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrNonSysCheckError() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrNonSysGetBin(u64 encoded_key, int a2, void* out, size_t out_size) {
    LOG_INFO(Lib_RegMgr, "called, encoded_key = {}, a2 = {}, out = {}, out_size = {}", encoded_key,
             a2, static_cast<void*>(out), out_size);
    return sceRegMgrGetBin(decode(encoded_key), out, out_size);
}

int PS4_SYSV_ABI sceRegMgrNonSysGetInt(u64 encoded_key, int a2, int* out) {
    LOG_INFO(Lib_RegMgr, "called, encoded_key = {}, a2 = {}, out = {}", encoded_key, a2,
             static_cast<void*>(out));
    return sceRegMgrGetInt(decode(encoded_key), out);
}

int PS4_SYSV_ABI sceRegMgrNonSysGetStr(u64 encoded_key, int a2, char* out, size_t out_size) {
    LOG_INFO(Lib_RegMgr, "called, encoded_key = {}, a2 = {}, out = {}, out_size = {}", encoded_key,
             a2, static_cast<void*>(out), out_size);
    return sceRegMgrGetStr(decode(encoded_key), out, out_size);
}

int PS4_SYSV_ABI sceRegMgrNonSysSetBin() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrNonSysSetInt() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrNonSysSetStr() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrPrintInfo() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrRecoverRegNvs() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrResetVal() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSetBin() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSetInitLevel() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSetInt() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSetStr() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSrvCnvRegionInt() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSrvCnvRegionStr() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSrvGetMachineType() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSrvGetQAFforReg() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSrvGetRealMachineType() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSrvGetRegion() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrSrvGetRegionStr() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrToolDataCheckGet() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrToolGetEntryCnt() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrToolGetInfo() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI sceRegMgrToolGetUpdateCnt() {
    LOG_ERROR(Lib_RegMgr, "(STUBBED) called");
    return ORBIS_OK;
}

void RegisterlibSceRegMgr(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("9BhoVC0q85k", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrBackupNeedMem);
    LIB_FUNCTION("f-qy0soAkfA", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrBackupPullData);
    LIB_FUNCTION("gqPjTzsu9vU", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrBackupPushData);
    LIB_FUNCTION("LZBANaFbPqM", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrCheckError);
    LIB_FUNCTION("BJi3VSFP+8s", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrCntlDeleteReg);
    LIB_FUNCTION("18ZCDYD5xjI", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrCntlGetFileName);
    LIB_FUNCTION("Ena7T6bVG5s", "libSceRegMgr", 1, "libSceRegMgr", 1, 1,
                 sceRegMgrCntlGetFilesCount);
    LIB_FUNCTION("tqHQdMPde4E", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrCntlStart);
    LIB_FUNCTION("Pj8CwRqcOIk", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrCntlStopHdd);
    LIB_FUNCTION("HuVpMjidh7c", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrCntSetInitReinst);
    LIB_FUNCTION("Ds-BHoPDTHY", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrDrvDataCheckGet);
    LIB_FUNCTION("HCUhvdmryuQ", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrDrvDataClose);
    LIB_FUNCTION("okzABktskwU", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrDrvDataOpen);
    LIB_FUNCTION("lwQpbDXKpbg", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrDrvGetEntCnt);
    LIB_FUNCTION("NwwkHSVFYBA", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrDrvGetUpdateCnt);
    LIB_FUNCTION("grCFSFvMxgU", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrEvtGetCnt);
    LIB_FUNCTION("djHSzoTfixE", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrEvtGetRegId);
    LIB_FUNCTION("NqxMleeTiLs", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrGetBin);
    LIB_FUNCTION("dhuH8HjNhUY", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrGetBinInitVal);
    LIB_FUNCTION("mPYKD12UDQI", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrGetInt);
    LIB_FUNCTION("i64ig3BF6f8", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrGetIntInitVal);
    LIB_FUNCTION("CTplLrrndUg", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrGetStr);
    LIB_FUNCTION("jqyhb1oMgHw", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrGetStrInitVal);
    LIB_FUNCTION("q+lPgqZniW4", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrGetVersion);
    LIB_FUNCTION("xhrI4zhlBuA", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrIsChange);
    LIB_FUNCTION("rebo0q4yREE", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrIsInitOK);
    LIB_FUNCTION("yfNwXqOshk0", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrLogPull);
    LIB_FUNCTION("HGxgXwGSAzQ", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrLogStart);
    LIB_FUNCTION("Eq+fDFaK3i0", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrNonSysCheckError);
    LIB_FUNCTION("k9LC1z8kh-E", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrNonSysGetBin);
    LIB_FUNCTION("dKeshzt29G4", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrNonSysGetInt);
    LIB_FUNCTION("DKWSr89zMsI", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrNonSysGetStr);
    LIB_FUNCTION("6yU1KEvOl14", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrNonSysSetBin);
    LIB_FUNCTION("PPRCIaNpJJY", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrNonSysSetInt);
    LIB_FUNCTION("AxUlf9UUxCg", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrNonSysSetStr);
    LIB_FUNCTION("hApQ4NmZ0IU", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrPrintInfo);
    LIB_FUNCTION("Xf2JqV9Xj2w", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrRecoverRegNvs);
    LIB_FUNCTION("eL7sMZaIdac", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrResetVal);
    LIB_FUNCTION("eXxrXkrLXa4", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSetBin);
    LIB_FUNCTION("YRPHqS8TcnI", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSetInitLevel);
    LIB_FUNCTION("khaYelw1Ytc", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSetInt);
    LIB_FUNCTION("ffInidSqRss", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSetStr);
    LIB_FUNCTION("CUSk0qEDj7s", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSrvCnvRegionInt);
    LIB_FUNCTION("PwmjM-dGut4", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSrvCnvRegionStr);
    LIB_FUNCTION("04zv+0zNJkg", "libSceRegMgr", 1, "libSceRegMgr", 1, 1,
                 sceRegMgrSrvGetMachineType);
    LIB_FUNCTION("wQlp94zzrWg", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSrvGetQAFforReg);
    LIB_FUNCTION("x0oTVOySvTU", "libSceRegMgr", 1, "libSceRegMgr", 1, 1,
                 sceRegMgrSrvGetRealMachineType);
    LIB_FUNCTION("sywg-RnhZMA", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSrvGetRegion);
    LIB_FUNCTION("xCxMQ7Efh4k", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrSrvGetRegionStr);
    LIB_FUNCTION("VmQkpRjBp3s", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrToolDataCheckGet);
    LIB_FUNCTION("voOYmhF1vsQ", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrToolGetEntryCnt);
    LIB_FUNCTION("0QcqLS+gFiQ", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrToolGetInfo);
    LIB_FUNCTION("jQ0656do1V4", "libSceRegMgr", 1, "libSceRegMgr", 1, 1, sceRegMgrToolGetUpdateCnt);
};

} // namespace Libraries::RegMgr