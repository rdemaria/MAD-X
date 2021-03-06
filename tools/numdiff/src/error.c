/*
 o---------------------------------------------------------------------o
 |
 | Ndiff
 |
 | Copyright (c) 2012+ laurent.deniau@cern.ch
 | Gnu General Public License
 |
 o---------------------------------------------------------------------o

   Purpose:
     manage error level and log error messages

 o---------------------------------------------------------------------o
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "error.h"

struct logmsg_config logmsg_config = {
  .level  = inform_level,
  .locate = 0,
  .flush  = 1             // always flush files
};

void
logmsg(unsigned level, const char *file, int line, const char *fmt, ...)
{
  static const char *str[] = { "trace: ", "debug: ", "", "warng: ", "error: ", "abort: " };
  enum { str_n = sizeof str/sizeof *str };

  assert(level < str_n);
  assert(fmt);

  if (logmsg_config.flush) fflush(stdout);

  if (level < inform_level || logmsg_config.locate) {
    const char *p = strrchr(file, '/');
    if (p) file = p+1;
    fprintf(stderr, "%s(%s:%d): ", str[level], file, line);
  } else
    fprintf(stderr, "%s", str[level]);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  putc('\n', stderr);

  if (logmsg_config.flush) fflush(stderr);

  switch(level) {
  case error_level: quit(EXIT_FAILURE);
  case abort_level: (abort)();
  }
}

