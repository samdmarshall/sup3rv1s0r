//
//  nuke.h
//  sup3rv1s0r
//
//  Created by Samantha Demi on 4/29/14.
//  Copyright (c) 2014 Samantha Demi. All rights reserved.
//

#ifndef sup3rv1s0r_nuke_h
#define sup3rv1s0r_nuke_h

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>
#include "attributes.h"

struct nuke_backup_item {
	char * name;
	CFTypeRef item;
} ATR_PACK;

struct nuke_backup {
	struct nuke_backup_item info_plist;
	struct nuke_backup_item status_plist;
	struct nuke_backup_item manifest_plist;
	struct nuke_backup_item udid_plist;
	struct nuke_backup_item manifest_mbdb;
} ATR_PACK;

struct nuke_backup * GenerateNukeBackup(SDMMD_AMDeviceRef device);

void NukeBackupRelease(struct nuke_backup * backup);

#endif
