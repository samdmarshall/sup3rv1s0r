//
//  host_sec_cert.c
//  sup3rv1s0r
//
//  Created by Samantha Demi on 4/29/14.
//  Copyright (c) 2014 Samantha Demi. All rights reserved.
//

#ifndef sup3rv1s0r_host_sec_cert_c
#define sup3rv1s0r_host_sec_cert_c

#include "host_sec_cert.h"
#include <Security/Security.h>
#include "GetPrimaryMACAddress.h"

Boolean LookupAppleConfiguratorCert(CFDataRef * host_cert) {
	Boolean found_cert = false;
	CFTypeRef results = NULL;
	CFMutableDictionaryRef search = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFDictionaryAddValue(search, kSecClass, kSecClassCertificate);
	CFStringRef mac_address = GetStringOfPrimaryMACAddress();
	CFStringRef host_cert_name = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("Apple Configurator (%@)"), mac_address);
	CFDictionaryAddValue(search, kSecAttrLabel, host_cert_name);
	OSStatus status = SecItemCopyMatching(search, &results);
	if (status == errSecSuccess) {
		status = SecItemExport(results, kSecFormatX509Cert, 0, NULL, host_cert);
		if (status == errSecSuccess) {
			found_cert = true;
		}
	}
	return found_cert;
}

#endif