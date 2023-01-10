#include "MediaFactoryInterface.h"

#define MEDIA_FACTORY_INTERFACE_OUT true, 1
#define MEDIA_FACTORY_INTERFACE_ERR true, 1, true, __FILE__, __LINE__

void MediaFactoryInterface::unknown() {
}

bool MediaFactoryInterface::setupExtensionCallback(unsigned char* unknown, RegisterInterface* registerInterface) {
	consoleLog("Setting Up Media Factory Interface Extension Callback", MEDIA_FACTORY_INTERFACE_OUT);
	return true;
}

void MediaFactoryInterface::closeExtensionCallback(ID mediaFactoryID) {
	consoleLog("Closing Media Factory Interface Extension Callback", MEDIA_FACTORY_INTERFACE_OUT);
}