#ifndef __DEBUG_HPP_
#define __DEBUG_HPP_

#include <windows.h>
#include <iostream>
#include <string>
#include "../MUtils/MUtils/String.hpp"

class Debug
{
public:
	Debug() = default;
	~Debug() = default;

	static	void Print(MString value);
	//static	void	Return() { OutputDebugStringW(s2ws("\n").c_str()); }
	static  std::wstring s2ws(MString const&);
};

#endif /*__DEBUG_HPP__*/