#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
//#include <stdlib.h>
 
#define APP_LOG(format, ...)    {printf(format, ##__VA_ARGS__); printf("\n");}
//#define APP_LOG_TS(format, ...)   {printf(format, ##__VA_ARGS__); printf("\n");}
//#define APP_LOG_WARN(format, ...)
#define APP_LOG_LINE()          APP_LOG("%s:%d %s", __FILE__, __LINE__, __FUNCTION__)
 
#endif
