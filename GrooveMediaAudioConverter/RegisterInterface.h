#pragma once
#include "shared.h"
#include "GMACodecMediaFactory.h"
#include <string>

class RegisterInterface {
	public:
	RegisterInterface(std::string fromFileName, std::string toFileName);
	virtual void __thiscall unknown(bool unknown);
	virtual bool __thiscall setupExtensionCallback(ID mediaFactoryID, bool unknown, GMACodecMediaFactory* gmaCodecMediaFactory);
	virtual void __thiscall closeExtensionCallback();
	private:
	std::string fromFileName = "";
	std::string toFileName = "";
};