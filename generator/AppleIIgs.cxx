/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2015 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "AppleIIgs.h"

#define PUSH() \
  fprintf(out, "; PUSH\n"); \
  fprintf(out, "  sta stack,x\n"); \
  fprintf(out, "  dex\n"); \
  fprintf(out, "  dex\n")

#define POP() \
  fprintf(out, "; POP\n"); \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  inx\n"); \
  fprintf(out, "  lda stack,x\n")

AppleIIgs::AppleIIgs()
{
  start_org = 0x1000;
}

AppleIIgs::~AppleIIgs()
{
}

int AppleIIgs::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65816\n");

  // stack location
  fprintf(out, "stack equ 0x4000\n");

  // ram start
//FIXME this is not correct
  fprintf(out, "ram_start equ 0x4200\n");
  fprintf(out, "heap_ptr equ ram_start\n");

  // points to locals
  fprintf(out, "locals equ 0x00\n");

  // temp variables
  fprintf(out, "result equ 0x02\n");
  fprintf(out, "remainder equ 0x04\n");
  fprintf(out, "length equ 0x06\n");
  fprintf(out, "value1 equ 0x08\n");
  fprintf(out, "value2 equ 0x10\n");
  fprintf(out, "value3 equ 0x12\n");
  fprintf(out, "address equ 0x14\n");

  // start
  fprintf(out, ".org 0x%04x\n", start_org);
  fprintf(out, "; change to 16-bit mode\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  xce\n");
  fprintf(out, "; all 16-bit registers\n");
  fprintf(out, "  rep #0x30\n");
  fprintf(out, "; set up processor stack\n");
  fprintf(out, "  lda #0x1FF\n");
  fprintf(out, "  tcs\n");
  fprintf(out, "; clear java stack\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  ldx #0\n");
  fprintf(out, "clear_java_stack:\n");
  fprintf(out, "  sta stack,x\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  inx\n");
  fprintf(out, "  cpx #0x100\n");
  fprintf(out, "  bne clear_java_stack\n");
  fprintf(out, "; set up java stack pointer\n");
  fprintf(out, "  ldx #0xFE\n");

  return 0;
}

int AppleIIgs::appleiigs_plotChar_IC()
{
//FIXME broked
  fprintf(out,
    "  ;; plotChar()\n"
    "ply\n"
    "pla\n"
    "sep #0x20\n"
    "sta 0x0000,y\n"
    "rep #0x20\n");

  return 0;
}

int AppleIIgs::appleiigs_printChar_C()
{
//FIXME broked
  fprintf(out, ";; printChar()\n");
  fprintf(out, "  pha\n");
  fprintf(out, "  phx\n");
  fprintf(out, "  phy\n");
  POP();
  fprintf(out,
    "  ora #0x80\n"
    "  sep #0x30\n"
    "  sec\n"
    "  xce\n"
    "  jsr 0xfded\n"
    "  clc\n"
    "  xce\n"
    "  rep #0x30\n");
  fprintf(out, "  ply\n");
  fprintf(out, "  plx\n");
  fprintf(out, "  pla\n");

  return 0;
}

