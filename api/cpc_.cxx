/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 * CPC support by Carsten Dost  - https://github.com/deringenieur71
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "JavaClass.h"
#include "cpc_.h"

#define CHECK_FUNC(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->cpc_##funct##sig(); \
  }

#define CHECK_FUNC_CONST(funct,sig) \
  if (strcmp(#funct#sig, method_name) == 0) \
  { \
    return generator->cpc_##funct##sig(const_val); \
  }

#define CHECK_FUNC_CONST_2(funct,sig) \
  if (strcmp(#funct#sig, function) == 0) \
  { \
    return generator->cpc_##funct##sig(const_val1, const_val2); \
  }

int cpc(JavaClass *java_class, Generator *generator, char *method_name)
{
  CHECK_FUNC(beep,)
  CHECK_FUNC(color,_III)
  CHECK_FUNC(cls,)
  CHECK_FUNC(screen,_I)
  CHECK_FUNC(putChar,_C)
  CHECK_FUNC(setCursor,_II)
  CHECK_FUNC(plot,_III)
  CHECK_FUNC(draw,_III)

  return -1;
}

int cpc(JavaClass *java_class, Generator *generator, char *method_name, int const_val)
{
  //CHECK_FUNC_CONST(someFunction,_I)

  return -1;
}

int cpc(JavaClass *java_class, Generator *generator, char *function, int const_val1, int const_val2)
{
  //CHECK_FUNC_CONST_2(someFunction,_II)

  return -1;
}

