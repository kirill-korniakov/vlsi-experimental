#ifndef __ALLOCA_H__
#define __ALLOCA_H__

#include <malloc.h>

#define popen _popen
#define pclose _pclose
#define nanl(str) strtod("NAN()", (char **) NULL)

int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, int n);

#endif //__ALLOCA_H__

