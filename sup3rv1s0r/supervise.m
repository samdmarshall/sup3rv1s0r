//
//  supervise.m
//  sup3rv1s0r
//
//  Created by Samantha Demi on 4/29/14.
//  Copyright (c) 2014 Samantha Demi. All rights reserved.
//

#ifndef sup3rv1s0r_supervise_c
#define sup3rv1s0r_supervise_c

#include "supervise.h"
#include <SDMMobileDevice/SDMMobileDevice.h>
#include "attach.h"
#include "Core.h"
#include "keybag.h"
#include <Foundation/Foundation.h>
#include <stdint.h>
#include <SDMMobileDevice/SDMMD_MobileBackup2.h>
#include <SDMMobileDevice/SDMMD_Connection_Private.h>

bool SuperviseDevice(char * udid) {
	bool return_value = false;
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		sdmmd_return_t result = 0;
		
		result = SDMMD_AMDeviceConnect(device);
		result = SDMMD_AMDeviceStartSession(device);
		
		struct nuke_backup * backup_data = GenerateNukeBackup(device);
		
		SDMMD_AMConnectionRef conn = NULL;
		result = SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_BACKUP2), NULL, &conn);
		printf("Attempting to clear supervision record...\n");
		
		CFPropertyListRef response = NULL;
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		PrintCFType(response);
		
		CFPropertyListRef ok_response = (__bridge CFPropertyListRef)@[ @"DLMessageVersionExchange", @"DLVersionsOk", (__bridge NSNumber *)CFArrayGetValueAtIndex(response, 1)];
		
		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), ok_response, kCFPropertyListBinaryFormat_v1_0);
				
		CFPropertyListRef hello = (__bridge CFPropertyListRef)@[ @"DLMessageProcessMessage", @{
				@"MessageName" : @"Hello",
				@"SupportedProtocolVersions" : @[@(2.1)]
			}
		];
		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), hello, kCFPropertyListBinaryFormat_v1_0);

		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		PrintCFType(response);
		
		CFStringRef device_udid = SDMMD_AMDeviceCopyUDID(device);
		CFPropertyListRef restore_command = (__bridge CFPropertyListRef)@[ @"DLMessageProcessMessage", @{
																	 @"MessageName" : @"Backup",
																	 @"TargetIdentifier" : (__bridge NSString *)device_udid,
																	 @"SourceIdentifier" : @"Root",
																	 @"Options" : /*@{
																			 @"RestoreShouldReboot" : @NO,
																			 @"RestoreSystemFiles" : @YES,
																			 @"RestorePreserveSettings" : @YES,
																			 @"RemoveItemsNotRestored" : @NO
																			 }*/
																	 @{
																		 @"ForceFullBackup" : @YES
																	 }
																	 }
																 ];
		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), restore_command, kCFPropertyListBinaryFormat_v1_0);
		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		PrintCFType(response);
		CFSafeRelease(response);
		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		PrintCFType(response);
		
		CFStringRef status_plist = CFArrayGetValueAtIndex(CFArrayGetValueAtIndex(response, 1), 0);
		result = SDMMD_MB2SendFile(conn, status_plist, backup_data->status_plist.item);
		CFSafeRelease(response);
		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		PrintCFType(response);
		
		CFStringRef manifest_plist = CFArrayGetValueAtIndex(CFArrayGetValueAtIndex(response, 1), 0);
		result = SDMMD_MB2SendFile(conn, manifest_plist, backup_data->manifest_plist.item);
		CFSafeRelease(response);
		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		PrintCFType(response);
		
		CFStringRef manifest_mbdb = CFArrayGetValueAtIndex(CFArrayGetValueAtIndex(response, 1), 0);
		result = SDMMD_MB2SendFile(conn, manifest_mbdb, backup_data->manifest_mbdb.item);
		CFSafeRelease(response);
		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		PrintCFType(response);
		
		CFStringRef info_plist = CFArrayGetValueAtIndex(CFArrayGetValueAtIndex(response, 1), 0);
		result = SDMMD_MB2SendFile(conn, info_plist, backup_data->info_plist.item);
		CFSafeRelease(response);
		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);

		CFMutableArrayRef files_list = SDMMD_MB2StatusResponseMessage();
		CFMutableDictionaryRef files_dict = CFArrayGetValueAtIndex(files_list, 3);
		
		CFMutableDictionaryRef info_file_data = SDMMD_create_dict();
		CFDictionaryAddValue(info_file_data, CFSTR("DLFileSize"), (__bridge CFNumberRef)@(CFDataGetLength(backup_data->info_plist.item)));
		CFDictionaryAddValue(info_file_data, CFSTR("DLFileModificationDate"), CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent()));
		CFDictionaryAddValue(info_file_data, CFSTR("DLFileType"), CFSTR("DLFileTypeRegular"));
		CFDictionaryAddValue(files_dict, CFSTR("Info.plist"), info_file_data);

		CFMutableDictionaryRef manifest_mbdb_file_data = SDMMD_create_dict();
		CFDictionaryAddValue(manifest_mbdb_file_data, CFSTR("DLFileSize"), (__bridge CFNumberRef)@(CFDataGetLength(backup_data->manifest_mbdb.item)));
		CFDictionaryAddValue(manifest_mbdb_file_data, CFSTR("DLFileModificationDate"), CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent()));
		CFDictionaryAddValue(manifest_mbdb_file_data, CFSTR("DLFileType"), CFSTR("DLFileTypeRegular"));
		CFDictionaryAddValue(files_dict, CFSTR("Manifest.mbdb"), manifest_mbdb_file_data);
		
		CFMutableDictionaryRef manifest_file_data = SDMMD_create_dict();
		CFDictionaryAddValue(manifest_file_data, CFSTR("DLFileSize"), (__bridge CFNumberRef)@(CFDataGetLength(backup_data->manifest_plist.item)));
		CFDictionaryAddValue(manifest_file_data, CFSTR("DLFileModificationDate"), CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent()));
		CFDictionaryAddValue(manifest_file_data, CFSTR("DLFileType"), CFSTR("DLFileTypeRegular"));
		CFDictionaryAddValue(files_dict, CFSTR("Manifest.plist"), manifest_file_data);

		CFMutableDictionaryRef status_info_data = SDMMD_create_dict();
		CFDictionaryAddValue(status_info_data, CFSTR("DLFileSize"), (__bridge CFNumberRef)@(CFDataGetLength(backup_data->status_plist.item)));
		CFDictionaryAddValue(status_info_data, CFSTR("DLFileModificationDate"), CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent()));
		CFDictionaryAddValue(status_info_data, CFSTR("DLFileType"), CFSTR("DLFileTypeRegular"));
		CFDictionaryAddValue(files_dict, CFSTR("Status.plist"), status_info_data);
		
		struct mbdb_file * manifest = ParseMBDBData(backup_data->manifest_mbdb.item);
		for (uint32_t record_index = 0; record_index < manifest->record_count; record_index++) {
			struct mbdb_file_name * file_hash = FindFileForFromManifestRecord(&(manifest->record[record_index]));
			if (file_hash->has_file) {
				unsigned char * udid_plist_hash = DataToSHA1(backup_data->udid_plist.item);
				if (memcmp(file_hash->hash, udid_plist_hash, sizeof(unsigned char[HASH_LENGTH])) == 0) {
					CFStringRef file_name = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"), file_hash->name[0], file_hash->name[1], file_hash->name[2], file_hash->name[3], file_hash->name[4], file_hash->name[5], file_hash->name[6], file_hash->name[7], file_hash->name[8], file_hash->name[9], file_hash->name[10], file_hash->name[11], file_hash->name[12], file_hash->name[13], file_hash->name[14], file_hash->name[15], file_hash->name[16], file_hash->name[17], file_hash->name[18], file_hash->name[19]);
					CFMutableDictionaryRef file_data = SDMMD_create_dict();
					CFDictionaryAddValue(file_data, CFSTR("DLFileSize"), CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt64Type, &(manifest->record[record_index].info.file_length)));
					CFDictionaryAddValue(file_data, CFSTR("DLFileModificationDate"), CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent()));
					CFDictionaryAddValue(file_data, CFSTR("DLFileType"), CFSTR("DLFileTypeRegular"));
					
					CFDictionaryAddValue(files_dict, file_name, file_data);
				}
				free(udid_plist_hash);
			}
			free(file_hash);
		}
		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), files_list, kCFPropertyListBinaryFormat_v1_0);
		CFSafeRelease(files_list);
		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		
		CFMutableArrayRef free_space = SDMMD_MB2StatusResponseMessage();
		uint64_t code_value = 0;
		CFNumberRef code = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt64Type, &code_value);
		CFArraySetValueAtIndex(free_space, 3, code);
		CFSafeRelease(code);
		
		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), free_space, kCFPropertyListBinaryFormat_v1_0);
		CFSafeRelease(free_space);

		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		
		CFStringRef restore_config = CFArrayGetValueAtIndex(CFArrayGetValueAtIndex(response, 1), 0);
		result = SDMMD_MB2SendFile(conn, restore_config, backup_data->udid_plist.item);
		CFSafeRelease(response);
		
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		CFDictionaryRef error_response = CFArrayGetValueAtIndex(response, 1);
		CFNumberRef error_code = CFDictionaryGetValue(error_response, CFSTR("ErrorCode"));
		uint64_t error = 0;
		CFNumberGetValue(error_code, kCFNumberSInt64Type, &error);
		return_value = (error == 0 ? true : false);
		if (error == 0) {
			printf("Successfully cleared device supervision settings...\n");
		}
		else {
			printf("Could not clear device supervision, please reboot and try again\n");
		}
		
		CFPropertyListRef goodbye = (__bridge CFPropertyListRef)@[ @"DLMessageDisconnect" ];
		SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), goodbye, kCFPropertyListBinaryFormat_v1_0);
		
		SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		
		SDMMD_AMDServiceConnectionInvalidate(conn);
		SDMMD_AMDeviceStopSession(device);
		SDMMD_AMDeviceDisconnect(device);
		
		NukeBackupRelease(backup_data);
	}
	return return_value;
}

#endif
