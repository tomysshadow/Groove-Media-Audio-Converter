#pragma once
#include "main.h"

struct RegisterInterface;

struct MediaFactoryInterface {
	virtual void __thiscall unknown();
	virtual bool __thiscall setupExtensionCallback(unsigned char* unknown, RegisterInterface* registerInterface);
	virtual void __thiscall closeExtensionCallback(ID mediaFactoryID);
};