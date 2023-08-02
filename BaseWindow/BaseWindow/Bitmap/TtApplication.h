#pragma once
#include "../framework.h"
#include <wingdi.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>

class TtApplication
{
public:
	std::string		mRoot;
public:
	static TtApplication* Get();

	bool Initialize();

	std::string GetAbsPath(const char* path) {
		return mRoot + path;
	}
};