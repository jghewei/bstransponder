#ifndef _DAN_OS_H_
#define _DAN_OS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include "Log.h"
#include "CMutex.h"

#define VERSION     "0.0.0"
std::string GetVersion(void);

std::string to_string(int d);
std::string to_string(float f);
std::string to_string(uint64_t llu);
std::string to_string(bool b);

bool IsFileExist(const char *fname);

#endif
