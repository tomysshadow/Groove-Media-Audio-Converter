#pragma once
#include "main.h"
#include "GMACodecMediaFactory.h"

struct RegisterInterface {
	virtual void __thiscall unknown(bool unknown);
	virtual bool __thiscall setupExtensionCallback(ID mediaFactoryID, bool unknown, GMACodecMediaFactory* gmaCodecMediaFactory);
	virtual void __thiscall closeExtensionCallback();
};