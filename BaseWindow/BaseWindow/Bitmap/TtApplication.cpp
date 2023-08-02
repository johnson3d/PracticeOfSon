#include "TtApplication.h"

TtApplication* TtApplication::Get()
{
	static TtApplication obj;
	return &obj;
}

bool TtApplication::Initialize()
{
	char path[256];
	::GetCurrentDirectoryA(sizeof(path), path);
	mRoot = path;

	return true;
}