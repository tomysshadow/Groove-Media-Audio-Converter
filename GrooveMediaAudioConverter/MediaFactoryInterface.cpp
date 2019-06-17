#include "MediaFactoryInterface.h"
#include "main.h"

void MediaFactoryInterface::unknown() {
	consoleLog("Unknown of Media Factory Interface");
	return;
}

bool MediaFactoryInterface::setupExtensionCallback(unsigned char* unknown, RegisterInterface* registerInterface) {
	consoleLog("Setting Up Extension Callback of Media Factory Interface");
	return true;
}

void MediaFactoryInterface::closeExtensionCallback(ID mediaFactoryID) {
	consoleLog("Closing Extension Callback of Media Factory Interface");
	return;
}