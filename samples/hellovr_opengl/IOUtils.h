#pragma once

#include <string>

long int SysTimeMS();

std::string readFile(std::string filename);

void dprintf(const char *fmt, ...);
