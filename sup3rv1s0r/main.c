//
//  main.c
//  sup3rv1s0r
//
//  Created by Samantha Demi on 4/29/14.
//  Copyright (c) 2014 Samantha Demi. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <getopt.h>

#include <SDMMobileDevice/SDMMobileDevice.h>
#include "supervise.h"
#include "list.h"
#include "install.h"

static char * helpArg = "-h,--help";
static char * listArg = "-l,--list";
static char * deviceArg = "-d,--device";
static char * superviseArg = "-s,--supervise";
static char * unsuperviseArg = "-u,--unsupervise";
static char * profileArg = "-p,--profile";

enum ToolOptions {
	OptionsHelp = 0x0,
	OptionsList,
	OptionsDevice,
	OptionsSupervise,
	OptionsUnsupervise,
	OptionsProfile,
	OptionsCount
};

static struct option long_options[OptionsCount] = {
	{"help", no_argument, 0x0, 'h'},
	{"list", no_argument, 0x0, 'l'},
	{"device", required_argument, 0x0, 'd'},
	{"supervise", no_argument, 0x0, 's'},
	{"unsupervise", no_argument, 0x0, 'u'},
	{"profile", required_argument, 0x0, 'p'}
};

static bool optionsEnable[OptionsCount] = {};

int main(int argc, const char * argv[]) {
	char * udid = NULL;
	char * profile_path = NULL;
	bool searchArgs = true;
	int counter = 0;
	while (searchArgs) {
		int option_index = 0x0;
		counter = getopt_long (argc, (char * const *)argv, "hld:sup:",long_options, &option_index);
		if (counter == -1) {
			break;
		}
		switch (counter) {
			case 'h': {
				optionsEnable[OptionsHelp] = true;
				searchArgs = false;
				break;
			}
			case 'l': {
				optionsEnable[OptionsList] = true;
				searchArgs = false;
				break;
			}
			case 'd': {
				if (optarg && !optionsEnable[OptionsDevice]) {
					optionsEnable[OptionsDevice] = true;
					udid = optarg;
				}
				break;
			}
			case 's': {
				if (!optionsEnable[OptionsSupervise] && !optionsEnable[OptionsUnsupervise]) {
					optionsEnable[OptionsSupervise] = true;
				}
				break;
			}
			case 'u': {
				if (!optionsEnable[OptionsUnsupervise] && !optionsEnable[OptionsSupervise]) {
					optionsEnable[OptionsUnsupervise] = true;
				}
				break;
			}
			case 'p': {
				if (optarg && !optionsEnable[OptionsProfile]) {
					profile_path = optarg;
					optionsEnable[OptionsProfile] = true;
				}
				break;
			}
			default: {
				printf("--help for help\n");
				break;
			};
		}
	}
	if (optionsEnable[OptionsHelp]) {
		printf("%s : help\n",helpArg);
		printf("%s : list attached devices\n",listArg);
		printf("%s [UDID] : specify a device\n",deviceArg);
		printf("%s : supervise a device\n",superviseArg);
		printf("%s : unsupervise a device\n",unsuperviseArg);
		printf("%s [.mobileconfig path] : install specificed .mobileconfig to a device\n",profileArg);
	}
	else {
		CFDataRef host_supervisor_cert = NULL;
		Boolean lookup_cert = LookupAppleConfiguratorCert(&host_supervisor_cert);
		if (lookup_cert) {
			if (optionsEnable[OptionsList]) {
				ListConnectedDevices();
			}
			if (optionsEnable[OptionsDevice]) {
				if (optionsEnable[OptionsUnsupervise] || optionsEnable[OptionsSupervise]) {
					optionsEnable[OptionsSupervise] = SuperviseDevice(udid);
				}
				if (optionsEnable[OptionsProfile] && optionsEnable[OptionsSupervise]) {
					InstallProfileToDevice(udid, profile_path, host_supervisor_cert);
				}
			}
		}
		else {
			printf("Could not find \"Apple Configurator\" cert in the keychain, please install Apple Configurator and try again.\n");
		}
		CFSafeRelease(host_supervisor_cert);
	}
	
    return 0;
}

