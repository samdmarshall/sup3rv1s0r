//
//  install.c
//  iOSConsole
//
//  Created by Samantha Demi on 4/21/14.
//  Copyright (c) 2014 Samantha Demi. All rights reserved.
//

#ifndef iOSConsole_install_c
#define iOSConsole_install_c

#include "install.h"
#include <SDMMobileDevice/SDMMobileDevice.h>
#include <CoreFoundation/CoreFoundation.h>
#include "attach.h"
#include "Core.h"

CFMutableDictionaryRef CreateCloudConfigutation(CFDataRef host_cert) {
	CFMutableDictionaryRef cloud_config = SDMMD_create_dict();
	
	CFDictionaryAddValue(cloud_config, CFSTR("AllowPairing"), kCFBooleanTrue);
	
	CFArrayRef anchor_certs = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	CFDictionaryAddValue(cloud_config, CFSTR("AnchorCertificates"), anchor_certs);
	CFSafeRelease(anchor_certs);
	
	CFDictionaryAddValue(cloud_config, CFSTR("IsSupervised"), kCFBooleanTrue);
	
	CFDictionaryAddValue(cloud_config, CFSTR("OrganizationName"), CFSTR("BadThings"));
	
	CFMutableArrayRef supervisor_certs = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	CFArrayAppendValue(supervisor_certs, host_cert);
	CFDictionaryAddValue(cloud_config, CFSTR("SupervisorHostCertificates"), supervisor_certs);
	CFSafeRelease(supervisor_certs);
	
	return cloud_config;
}

void InstallProfileToDevice(char * udid, char * path, CFDataRef host_cert) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		sdmmd_return_t result = 0;
//		
//		SDMMD_AMDeviceConnect(device);
//		SDMMD_AMDeviceStartSession(device);
//		SDMMD_AMConnectionRef conn = NULL;
//		SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_MCINSTALL), NULL, &conn);
//		printf("Attempting to install new supervision record...\n");
//		
//		CFMutableDictionaryRef flush = SDMMD_create_dict();
//		CFDictionaryAddValue(flush, CFSTR("RequestType"), CFSTR("Flush"));
//		
//		CFMutableDictionaryRef cloud_request = SDMMD_create_dict();
//		CFDictionaryAddValue(cloud_request, CFSTR("RequestType"), CFSTR("SetCloudConfiguration"));
//		CFMutableDictionaryRef cloud_config = CreateCloudConfigutation(host_cert);
//		CFDictionaryAddValue(cloud_request, CFSTR("CloudConfiguration"), cloud_config);
//		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), cloud_request, kCFPropertyListXMLFormat_v1_0);
//		CFSafeRelease(cloud_request);
//		CFSafeRelease(cloud_config);
//		
//		CFPropertyListRef response = NULL;
//		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
//		if (CFDictionaryContainsKey(response, CFSTR("Status"))) {
//			if (CFStringCompare(CFSTR("Acknowledged"), CFDictionaryGetValue(response, CFSTR("Status")), 0) == kCFCompareEqualTo) {
//				printf("Successfully set new supervision record...\n");
//			}
//			else {
//				printf("Device rejected new supervision record.\n");
//			}
//		}
//		CFSafeRelease(response);
//		
//		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), flush, kCFPropertyListXMLFormat_v1_0);
//		response = NULL;
//		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
//		CFSafeRelease(response);
//		
//		CFMutableDictionaryRef escalate_request = SDMMD_create_dict();
//		CFDictionaryAddValue(escalate_request, CFSTR("RequestType"), CFSTR("Escalate"));
//		CFDictionaryAddValue(escalate_request, CFSTR("SupervisorCertificate"), host_cert);
//		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), escalate_request, kCFPropertyListXMLFormat_v1_0);
//		CFSafeRelease(escalate_request);
//		
//		response = NULL;
//		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
//		PrintCFType(response);
//		CFSafeRelease(response);
//		
//		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), flush, kCFPropertyListXMLFormat_v1_0);
//		response = NULL;
//		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
//		CFSafeRelease(response);
//		
//		// there needs to be a challenge response here
//		
//		
//		CFDataRef config_data = CFDataCreateFromFilePath(path);
//		
//		CFMutableDictionaryRef request = SDMMD_create_dict();
//		CFDictionaryAddValue(request, CFSTR("RequestType"), CFSTR("InstallProfileSilent"));
//		CFDictionaryAddValue(request, CFSTR("Payload"), config_data);
//		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), request, kCFPropertyListXMLFormat_v1_0);
//		CFSafeRelease(request);
//		CFSafeRelease(config_data);
//		
//		response = NULL;
//		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
//		PrintCFType(response);
//		CFSafeRelease(response);
//		
//		SDMMD_AMDServiceConnectionInvalidate(conn);
//		SDMMD_AMDeviceStopSession(device);
//		SDMMD_AMDeviceDisconnect(device);
		
	
	}
}

#endif
