//
//  attach.h
//  iOSConsole
//
//  Created by Samantha Demi on 1/4/14.
//  Copyright (c) 2014 Samantha Demi. All rights reserved.
//

#ifndef iOSConsole_attach_h
#define iOSConsole_attach_h

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>

SDMMD_AMDeviceRef FindDeviceFromUDID(char * udid);
SDMMD_AMConnectionRef AttachToDeviceAndService(SDMMD_AMDeviceRef device, char * service);

#endif
