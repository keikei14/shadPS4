// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"

namespace Core::Loader {
class SymbolsResolver;
}

namespace Libraries::RegMgr {

int PS4_SYSV_ABI sceRegMgrBackupNeedMem();
int PS4_SYSV_ABI sceRegMgrBackupPullData();
int PS4_SYSV_ABI sceRegMgrBackupPushData();
int PS4_SYSV_ABI sceRegMgrCheckError();
int PS4_SYSV_ABI sceRegMgrCntlDeleteReg();
int PS4_SYSV_ABI sceRegMgrCntlGetFileName();
int PS4_SYSV_ABI sceRegMgrCntlGetFilesCount();
int PS4_SYSV_ABI sceRegMgrCntlStart();
int PS4_SYSV_ABI sceRegMgrCntlStopHdd();
int PS4_SYSV_ABI sceRegMgrCntSetInitReinst();
int PS4_SYSV_ABI sceRegMgrDrvDataCheckGet();
int PS4_SYSV_ABI sceRegMgrDrvDataClose();
int PS4_SYSV_ABI sceRegMgrDrvDataOpen();
int PS4_SYSV_ABI sceRegMgrDrvGetEntCnt();
int PS4_SYSV_ABI sceRegMgrDrvGetUpdateCnt();
int PS4_SYSV_ABI sceRegMgrEvtGetCnt();
int PS4_SYSV_ABI sceRegMgrEvtGetRegId();
int PS4_SYSV_ABI sceRegMgrGetBin(u32 key, void* out, size_t out_size);
int PS4_SYSV_ABI sceRegMgrGetBinInitVal();
int PS4_SYSV_ABI sceRegMgrGetInt(u32 key, int* out);
int PS4_SYSV_ABI sceRegMgrGetIntInitVal();
int PS4_SYSV_ABI sceRegMgrGetStr(u32 key, char* out, size_t out_size);
int PS4_SYSV_ABI sceRegMgrGetStrInitVal();
int PS4_SYSV_ABI sceRegMgrGetVersion();
int PS4_SYSV_ABI sceRegMgrIsChange();
int PS4_SYSV_ABI sceRegMgrIsInitOK();
int PS4_SYSV_ABI sceRegMgrLogPull();
int PS4_SYSV_ABI sceRegMgrLogStart();
int PS4_SYSV_ABI sceRegMgrNonSysCheckError();
int PS4_SYSV_ABI sceRegMgrNonSysGetBin(u64 encoded_key, int a2, void* out, size_t out_size);
int PS4_SYSV_ABI sceRegMgrNonSysGetInt(u64 encoded_key, int a2, int* out);
int PS4_SYSV_ABI sceRegMgrNonSysGetStr(u64 encoded_key, int a2, char* out, size_t out_size);
int PS4_SYSV_ABI sceRegMgrNonSysSetBin();
int PS4_SYSV_ABI sceRegMgrNonSysSetInt();
int PS4_SYSV_ABI sceRegMgrNonSysSetStr();
int PS4_SYSV_ABI sceRegMgrPrintInfo();
int PS4_SYSV_ABI sceRegMgrRecoverRegNvs();
int PS4_SYSV_ABI sceRegMgrResetVal();
int PS4_SYSV_ABI sceRegMgrSetBin();
int PS4_SYSV_ABI sceRegMgrSetInitLevel();
int PS4_SYSV_ABI sceRegMgrSetInt();
int PS4_SYSV_ABI sceRegMgrSetStr();
int PS4_SYSV_ABI sceRegMgrSrvCnvRegionInt();
int PS4_SYSV_ABI sceRegMgrSrvCnvRegionStr();
int PS4_SYSV_ABI sceRegMgrSrvGetMachineType();
int PS4_SYSV_ABI sceRegMgrSrvGetQAFforReg();
int PS4_SYSV_ABI sceRegMgrSrvGetRealMachineType();
int PS4_SYSV_ABI sceRegMgrSrvGetRegion();
int PS4_SYSV_ABI sceRegMgrSrvGetRegionStr();
int PS4_SYSV_ABI sceRegMgrToolDataCheckGet();
int PS4_SYSV_ABI sceRegMgrToolGetEntryCnt();
int PS4_SYSV_ABI sceRegMgrToolGetInfo();
int PS4_SYSV_ABI sceRegMgrToolGetUpdateCnt();

void RegisterlibSceRegMgr(Core::Loader::SymbolsResolver* sym);
} // namespace Libraries::RegMgr