/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#ifndef _PARALLELLA_H
#define _PARALLELLA_H

#include "Generator.h"
#include "JavaClass.h"

int parallella(JavaClass *java_class, Generator *generator, char *method_name);
int parallella(JavaClass *java_class, Generator *generator, char *method_name, int const_val);
int parallella(JavaClass *java_class, Generator *generator, char *method_name, int const_val1, int const_val2);

#endif

