#include "DanOS.h"
#include <fcntl.h> 

std::string GetVersion()
{
    return VERSION;
}

std::string to_string(int d)
{
    char str[32] = {0};
    snprintf(str, 31, "%d", d);
    return str;
}

std::string to_string(float f)
{
    char str[32] = {0};
    snprintf(str, 31, "%.2f", f);
    return str;
}

std::string to_string(uint64_t llu)
{
    char str[32] = {0};
#ifdef __TARGET__
    snprintf(str, 31, "%llu", llu);
#else
    snprintf(str, 31, "%lu", llu);
#endif
    return str;
}

std::string to_string(bool b)
{
    return b ? "true" : "false";
}
 
bool IsFileExist(const char *fname)
{
    if ((access(fname, F_OK)) != -1)
    {
        return true; 
    }
    else
    {
        return false;
    }
}
