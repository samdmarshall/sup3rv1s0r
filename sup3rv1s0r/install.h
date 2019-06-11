//
//  install.h
//  iOSConsole
//
//  Created by Samantha Demi on 4/21/14.
//  Copyright (c) 2014 Samantha Demi. All rights reserved.
//

#ifndef iOSConsole_install_h
#define iOSConsole_install_h

#include <CoreFoundation/CoreFoundation.h>

void InstallProfileToDevice(char * udid, char * path, CFDataRef host_cert);

#endif
