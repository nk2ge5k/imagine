#ifndef DEBUG_H
#define DEBUG_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "types.h"

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define put_prefix_(dst) fprintf(dst, "[%s:%d]: ", __FILENAME__, __LINE__)

#define STD_ERROR strerror(errno)

#define errorf(format, ...)                                                    \
  do {                                                                         \
    put_prefix_(stderr);                                                       \
    fprintf(stderr, "ERROR " format "\n", ##__VA_ARGS__);                      \
  } while (0)

#ifndef DEBUG

#define debugf(format, ...)
#define assertf(expression, format, ...)

#else

#include <stdlib.h>

#define debugf(format, ...)                                                    \
  do {                                                                         \
    put_prefix_(stderr);                                                       \
    fprintf(stderr, "DEBUG " format "\n", ##__VA_ARGS__);                      \
  } while (0)

#define assertf(expression, format, ...)                                       \
  do {                                                                         \
    if (!(expression)) {                                                       \
      errorf("Assertion (" #expression ") failed: " format, ##__VA_ARGS__);    \
      abort();                                                                 \
    }                                                                          \
  } while (0)

#endif

#ifdef __cplusplus
extern "C" {
#endif

i64 ustime(void);
i64 mstime(void);

void beginTimeProfile();
void endTimeProfile(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
