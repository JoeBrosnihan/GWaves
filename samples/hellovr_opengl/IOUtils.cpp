
#include "IOUtils.h"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdarg>
#include <Windows.h>
#include <debugapi.h>

long int SysTimeMS()
{
	FILETIME ft_now;
	GetSystemTimeAsFileTime(&ft_now);
	LONGLONG ll_now = (LONGLONG)ft_now.dwLowDateTime + ((LONGLONG)(ft_now.dwHighDateTime) << 32LL);
	return ll_now / 10000;
}

std::string readFile(std::string filename)
{
	std::cout << "Reading file: " << filename << std::endl;
	std::string contents = ""; // reading a large file in this way is inefficient
	std::ifstream f;
	f.open(filename);
	if (f.is_open()) {
		std::string line;
		while (std::getline(f, line)) {
			contents = contents + line + '\n';
		}
	}
	else
	{
		std::cout << "Unable to open file." << std::endl;
	}
	std::cout << contents << std::endl;
	return contents;
}

void dprintf(const char *fmt, ...)
{
	va_list args;
	char buffer[2048];

	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	va_end(args);

	printf("%s", buffer);

	OutputDebugStringA(buffer);
}