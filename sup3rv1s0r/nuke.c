//
//  nuke.c
//  sup3rv1s0r
//
//  Created by Samantha Demi on 4/29/14.
//  Copyright (c) 2014 Samantha Demi. All rights reserved.
//

#ifndef sup3rv1s0r_nuke_c
#define sup3rv1s0r_nuke_c

#include "nuke.h"
#include "mbdb.h"
#include "keybag.h"


#define kNukeBackUpName "NukeCloudConfig.iosdevicebackup"
#define kUDIDPlistLength 182

static UInt8 udid_plist[kUDIDPlistLength] = {
	0x3C, 0x3F, 0x78, 0x6D, 0x6C, 0x20, 0x76, 0x65, 0x72, 0x73,
	0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x2E, 0x30, 0x22, 0x20,
	0x65, 0x6E, 0x63, 0x6F, 0x64, 0x69, 0x6E, 0x67, 0x3D, 0x22,
	0x55, 0x54, 0x46, 0x2D, 0x38, 0x22, 0x3F, 0x3E, 0x0A, 0x3C,
	0x21, 0x44, 0x4F, 0x43, 0x54, 0x59, 0x50, 0x45, 0x20, 0x70,
	0x6C, 0x69, 0x73, 0x74, 0x20, 0x50, 0x55, 0x42, 0x4C, 0x49,
	0x43, 0x20, 0x22, 0x2D, 0x2F, 0x2F, 0x41, 0x70, 0x70, 0x6C,
	0x65, 0x2F, 0x2F, 0x44, 0x54, 0x44, 0x20, 0x50, 0x4C, 0x49,
	0x53, 0x54, 0x20, 0x31, 0x2E, 0x30, 0x2F, 0x2F, 0x45, 0x4E,
	0x22, 0x20, 0x22, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F,
	0x77, 0x77, 0x77, 0x2E, 0x61, 0x70, 0x70, 0x6C, 0x65, 0x2E,
	0x63, 0x6F, 0x6D, 0x2F, 0x44, 0x54, 0x44, 0x73, 0x2F, 0x50,
	0x72, 0x6F, 0x70, 0x65, 0x72, 0x74, 0x79, 0x4C, 0x69, 0x73,
	0x74, 0x2D, 0x31, 0x2E, 0x30, 0x2E, 0x64, 0x74, 0x64, 0x22,
	0x3E, 0x0A, 0x3C, 0x70, 0x6C, 0x69, 0x73, 0x74, 0x20, 0x76,
	0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x31, 0x2E,
	0x30, 0x22, 0x3E, 0x0A, 0x3C, 0x66, 0x61, 0x6C, 0x73, 0x65,
	0x2F, 0x3E, 0x0A, 0x3C, 0x2F, 0x70, 0x6C, 0x69, 0x73, 0x74,
	0x3E, 0x0A };

CFDataRef GenerateStatusPlist() {
	CFMutableDictionaryRef plist = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFDictionaryAddValue(plist, CFSTR("SnapshotState"), CFSTR("finished"));
	CFDictionaryAddValue(plist, CFSTR("Version"), CFSTR("2.4"));
	
	CFUUIDRef uuid = CFUUIDCreate(kCFAllocatorDefault);
	CFDictionaryAddValue(plist, CFSTR("UUID"), CFUUIDCreateString(kCFAllocatorDefault, uuid));
	CFSafeRelease(uuid);
	
	CFDictionaryAddValue(plist, CFSTR("IsFullBackup"), kCFBooleanFalse);
	CFDictionaryAddValue(plist, CFSTR("BackupState"), CFSTR("new"));
	
	CFDateRef current_date = CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent());
	CFDictionaryAddValue(plist, CFSTR("Date"), current_date);
	CFSafeRelease(current_date);
	
	return CFPropertyListCreateData(kCFAllocatorDefault, plist, kCFPropertyListXMLFormat_v1_0, 0, NULL);
}

CFDataRef GenerateInfoPlist(SDMMD_AMDeviceRef device) {
	CFMutableDictionaryRef plist = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	
	CFStringRef device_name = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
	CFDictionaryAddValue(plist, CFSTR("Device Name"), device_name);
	CFDictionaryAddValue(plist, CFSTR("Display Name"), device_name);
	CFSafeRelease(device_name);
	
	CFDateRef current_date = CFDateCreate(kCFAllocatorDefault, 0);
	CFDictionaryAddValue(plist, CFSTR("Last Backup Date"), current_date);
	CFSafeRelease(current_date);
	
	CFStringRef product_type = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductType));
	CFDictionaryAddValue(plist, CFSTR("Product Type"), product_type);
	CFSafeRelease(product_type);
	
	CFStringRef product_version = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductVersion));
	CFDictionaryAddValue(plist, CFSTR("Product Version"), product_version);
	CFSafeRelease(product_version);
	
	CFStringRef serial = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kSerialNumber));
	CFDictionaryAddValue(plist, CFSTR("Serial Number"), serial);
	CFSafeRelease(serial);
	
	CFStringRef udid = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kUniqueDeviceID));
	CFDictionaryAddValue(plist, CFSTR("Target Identifier"), udid);
	CFSafeRelease(udid);
	
	CFDictionaryAddValue(plist, CFSTR("Target Type"), CFSTR("Tahoe"));
	
	return CFPropertyListCreateData(kCFAllocatorDefault, plist, kCFPropertyListXMLFormat_v1_0, 0, NULL);
}

CFDictionaryRef CreateLockdownDictionary(SDMMD_AMDeviceRef device) {
	CFMutableDictionaryRef lockdown_dict = SDMMD_create_dict();
	
	CFStringRef product_type = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductType));
	CFDictionaryAddValue(lockdown_dict, CFSTR("ProductType"), product_type);
	CFSafeRelease(product_type);
	
	CFStringRef product_version = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductVersion));
	CFDictionaryAddValue(lockdown_dict, CFSTR("ProductVersion"), product_version);
	CFSafeRelease(product_version);
	
	CFStringRef serial = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kSerialNumber));
	CFDictionaryAddValue(lockdown_dict, CFSTR("SerialNumber"), serial);
	CFSafeRelease(serial);
	
	CFStringRef udid = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kUniqueDeviceID));
	CFDictionaryAddValue(lockdown_dict, CFSTR("UniqueDeviceID"), udid);
	CFSafeRelease(udid);
	
	CFStringRef device_name = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
	CFDictionaryAddValue(lockdown_dict, CFSTR("DeviceName"), device_name);
	CFSafeRelease(device_name);
	
	CFStringRef build_version = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kBuildVersion));
	CFDictionaryAddValue(lockdown_dict, CFSTR("BuildVersion"), build_version);
	CFSafeRelease(build_version);
	
	CFDictionaryAddValue(lockdown_dict, CFSTR("com.apple.mobile.iTunes.accessories"), SDMMD_create_dict());
	
	CFMutableDictionaryRef lockdown = SDMMD_create_dict();
	CFDictionaryAddValue(lockdown, CFSTR("EnableWifiConnections"), kCFBooleanTrue);
	CFDictionaryAddValue(lockdown_dict, CFSTR("com.apple.mobile.wireless_lockdown"), lockdown);
	CFSafeRelease(lockdown);
	
	CFMutableDictionaryRef accessibility = SDMMD_create_dict();
	CFDictionaryAddValue(accessibility, CFSTR("MonoAudioEnabledByiTunes"), kCFBooleanFalse);
	CFDictionaryAddValue(accessibility, CFSTR("VoiceOverTouchEnabledByiTunes"), kCFBooleanFalse);
	CFDictionaryAddValue(accessibility, CFSTR("ZoomTouchEnabledByiTunes"), kCFBooleanFalse);
	CFDictionaryAddValue(accessibility, CFSTR("SpeakAutoCorrectionsEnabledByiTunes"), kCFBooleanFalse);
	CFDictionaryAddValue(accessibility, CFSTR("InvertDisplayEnabledByiTunes"), kCFBooleanFalse);
	CFDictionaryAddValue(accessibility, CFSTR("ClosedCaptioningEnabledByiTunes"), kCFBooleanFalse);
	CFDictionaryAddValue(lockdown_dict, CFSTR("com.apple.Accessibility"), accessibility);
	CFSafeRelease(accessibility);
	
	CFDictionaryAddValue(lockdown_dict, CFSTR("com.apple.mobile.data_sync"), SDMMD_create_dict());
	
	CFDictionaryAddValue(lockdown_dict, CFSTR("com.apple.TerminalFlashr"), SDMMD_create_dict());
	
	CFDictionaryAddValue(lockdown_dict, CFSTR("com.apple.MobileDeviceCrashCopy"), SDMMD_create_dict());

	return lockdown_dict;
}

CFDataRef GenerateManifestPlist(SDMMD_AMDeviceRef device) {
	CFMutableDictionaryRef plist = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	
	CFMutableDataRef keybag = CFDataCreateMutable(kCFAllocatorDefault, 0);
	for (uint32_t index = 0; index < kNukeKeyBagCount; index++) {
		CFDataAppendBytes(keybag, (const UInt8 *)&(nuke_keybag[index].header), sizeof(struct keybag_block_header));
		CFDataAppendBytes(keybag, (const UInt8 *)(nuke_keybag[index].data), be32toh(nuke_keybag[index].header.length));
	}
	CFDictionaryAddValue(plist, CFSTR("BackupKeyBag"), keybag);
	CFSafeRelease(keybag);
	
	CFDictionaryAddValue(plist, CFSTR("Version"), CFSTR("9.1"));

	CFDateRef current_date = CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent());
	CFDictionaryAddValue(plist, CFSTR("Date"), current_date);
	CFSafeRelease(current_date);
	
	CFDictionaryAddValue(plist, CFSTR("SystemDomainsVersion"), CFSTR("20.0"));

	CFDictionaryAddValue(plist, CFSTR("WasPasscodeSet"), kCFBooleanFalse);

	CFDictionaryRef lockdown_dict = CreateLockdownDictionary(device);
	CFDictionaryAddValue(plist, CFSTR("Lockdown"), lockdown_dict);
	CFSafeRelease(lockdown_dict);

	CFDictionaryRef empty_dict = SDMMD_create_dict();
	CFDictionaryAddValue(plist, CFSTR("Applications"), empty_dict);
	CFSafeRelease(empty_dict);

	CFBooleanRef encrypted = SDMMD_AMDeviceCopyValue(device, CFSTR("com.apple.mobile.backup"), CFSTR("WillEncrypt"));
	CFDictionaryAddValue(plist, CFSTR("IsEncrypted"), encrypted);

	
	return CFPropertyListCreateData(kCFAllocatorDefault, plist, kCFPropertyListXMLFormat_v1_0, 0, NULL);
}

CFDataRef GenerateUDIDPlist() {
	return CFDataCreate(kCFAllocatorDefault, udid_plist, kUDIDPlistLength);
}

#define kInfoPlist "Info.plist"
#define kStatusPlist "Status.plist"
#define kManifestPlist "Manifest.plist"
#define kManifestDB "Manifest.mbdb"
#define kUUIDPlist "c003c8ddd80eef8c33251ed656572e681a84d463"

struct nuke_backup * GenerateNukeBackup(SDMMD_AMDeviceRef device) {
	printf("Generating Payload...\n");
	struct nuke_backup * nuke_backup = calloc(1, sizeof(struct nuke_backup));

	nuke_backup->info_plist.name = kInfoPlist;
	nuke_backup->info_plist.item = GenerateInfoPlist(device);
	
	nuke_backup->status_plist.name = kStatusPlist;
	nuke_backup->status_plist.item = GenerateStatusPlist();
	
	nuke_backup->manifest_plist.name = kManifestPlist;
	nuke_backup->manifest_plist.item = GenerateManifestPlist(device);
	
	nuke_backup->udid_plist.name = kUUIDPlist;
	nuke_backup->udid_plist.item = GenerateUDIDPlist();
	
	nuke_backup->manifest_mbdb.name = kManifestDB;
	nuke_backup->manifest_mbdb.item = PrepareManifestMBDB();

	return nuke_backup;
}

void NukeBackupRelease(struct nuke_backup * backup) {
	if (backup) {
		CFSafeRelease(backup->info_plist.item);
		
		CFSafeRelease(backup->status_plist.item);
		
		CFSafeRelease(backup->manifest_plist.item);
		
		CFSafeRelease(backup->udid_plist.item);
		
		CFSafeRelease(backup->manifest_mbdb.item);
	}
}


#endif
