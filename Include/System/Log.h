#ifndef _SYSTEM_LOG_H_
#define _SYSTEM_LOG_H_

#include <wchar.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#define LOG_DEBUG			0
#define LOG_INFORMATION		1
#define LOG_WARNING			2
#define LOG_CRITICAL		3

#define LOG_ALL			LOG_INFORMATION

void Sys_LogEntry(const char *module, uint8_t severity, const char *format, ...);

#endif /* _SYSTEM_LOG_H_ */

