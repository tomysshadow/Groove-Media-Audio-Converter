#pragma once
#include "shared.h"

class RegisterInterface;

class MediaFactoryInterface {
	public:
	virtual void __thiscall unknown();
	virtual bool __thiscall setupExtensionCallback(unsigned char* unknown, RegisterInterface* registerInterface);
	virtual void __thiscall closeExtensionCallback(ID mediaFactoryID);
};