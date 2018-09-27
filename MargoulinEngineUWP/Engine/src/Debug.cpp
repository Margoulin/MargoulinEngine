#include "Debug.hpp"

void	Debug::Print(MString value)
{
	OutputDebugStringW(s2ws(value).c_str());
}

std::wstring	Debug::s2ws(MString const& value)
{
	MString newVal = value + '\n';
	int len;
	int slength = (int)newVal.Count() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, newVal.Str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, newVal.Str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}