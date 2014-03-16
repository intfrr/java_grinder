/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "C64.h"

// ABI is:
// A
// X
// Y

#define PUSH_LO \
  fprintf(out, "; PUSH_LO\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  sta stack_lo,y\n")

#define PUSH_HI \
  fprintf(out, "; PUSH_HI\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  sta stack_hi,y\n"); \
  fprintf(out, "  dec SP\n")

#define POP_HI \
  fprintf(out, "; POP_HI\n"); \
  fprintf(out, "  inc SP\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  lda stack_hi,y\n")

#define POP_LO \
  fprintf(out, "; POP_LO\n"); \
  fprintf(out, "  ldy SP\n"); \
  fprintf(out, "  lda stack_lo,y\n")

#define POKE(dst) \
  POP_HI; \
  POP_LO; \
  fprintf(out, "  sta 0x%04x\n", dst)

#define PEEK(src) \
  fprintf(out, "  lda 0x%04x\n", src); \
  PUSH_LO; \
  fprintf(out, "  lda #0\n"); \
  PUSH_HI

C64::C64()
{

}

C64::~C64()
{
}

int C64::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".65xx\n");
  // heap
  fprintf(out, "ram_start equ 0x8000\n");
  fprintf(out, "heap_ptr equ ram_start\n");
  // for indirection (2 bytes)
  fprintf(out, "address equ 0xfb\n");
  // java stack
  fprintf(out, "stack_lo equ 0xc000\n");
  fprintf(out, "stack_hi equ 0xc100\n");
  fprintf(out, "SP equ 0xfd\n");
  // points to locals
  fprintf(out, "locals equ 0xfe\n");
  // temp variables
  fprintf(out, "result equ 0x20\n");
  fprintf(out, "return equ 0x22\n");
  fprintf(out, "remainder equ 0x24\n");
  fprintf(out, "length equ 0x26\n");
  fprintf(out, "temp equ 0x28\n");
  fprintf(out, "value1 equ 0x2a\n");
  fprintf(out, "value2 equ 0x2c\n");
  fprintf(out, "value3 equ 0x2e\n");
  fprintf(out, "value4 equ 0x30\n");
  // sprites
  fprintf(out, "sprite_msb_set equ 0x10\n");
  fprintf(out, "sprite_msb_clear equ 0x11\n");
  fprintf(out, "sprite_x equ 0x12\n");
  fprintf(out, "sprite_y equ 0x13\n");

  // fast multiply related
  fprintf(out, "product equ 0x40\n");
  fprintf(out, "square1_lo equ 0xc200\n");
  fprintf(out, "square1_hi equ 0xc400\n");
  fprintf(out, "square2_lo equ 0xc600\n");
  fprintf(out, "square2_hi equ 0xc800\n\n");

  // basic loader
  fprintf(out, ".org 0x07ff\n\n");

  fprintf(out, "dw 0x0801\n");
  fprintf(out, "dw start\n");
  fprintf(out, "dw 2013\n");
  fprintf(out, "db 0x9e\n");
  fprintf(out, "db (((start / 1000) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 100) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 10) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 1) %% 10) + 0x30)\n");
  fprintf(out, "db ':'\n");
  fprintf(out, "db 0x8f\n");
  fprintf(out, "db \" MORTIS\"\n");
  fprintf(out, "db 0\n");
  fprintf(out, "dw 0\n\n");

  fprintf(out, "start:\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  sta SP\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  txs\n");
  fprintf(out, "  jsr make_square_tables\n\n");

  return 0;
}

int C64::c64_sid_voice1_frequency(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd401\n");
  POP_LO;
  fprintf(out, "  sta 0xd400\n");

  return 0;
}

int C64::c64_sid_voice1_pulse_width(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd403\n");
  POP_LO;
  fprintf(out, "  sta 0xd402\n");

  return 0;
}

int C64::c64_sid_voice1_waveform(/* value */) { POKE(0xd404); return 0; }

int C64::c64_sid_voice1_adsr(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd406\n");
  POP_LO;
  fprintf(out, "  sta 0xd405\n");

  return 0;
}

int C64::c64_sid_voice2_frequency(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd408\n");
  POP_LO;
  fprintf(out, "  sta 0xd407\n");

  return 0;
}

int C64::c64_sid_voice2_pulse_width(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd40a\n");
  POP_LO;
  fprintf(out, "  sta 0xd409\n");

  return 0;
}

int C64::c64_sid_voice2_waveform(/* value */) { POKE(0xd40b); return 0; }

int C64::c64_sid_voice2_adsr(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd40d\n");
  POP_LO;
  fprintf(out, "  sta 0xd40c\n");

  return 0;
}

int C64::c64_sid_voice3_frequency(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd40f\n");
  POP_LO;
  fprintf(out, "  sta 0xd40e\n");

  return 0;
}

int C64::c64_sid_voice3_pulse_width(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd411\n");
  POP_LO;
  fprintf(out, "  sta 0xd410\n");

  return 0;
}

int C64::c64_sid_voice3_waveform(/* value */) { POKE(0xd412); return 0; }

int C64::c64_sid_voice3_adsr(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd414\n");
  POP_LO;
  fprintf(out, "  sta 0xd413\n");

  return 0;
}

int C64::c64_sid_filter_cutoff(/* value */)
{
  POP_HI;
  fprintf(out, "  sta 0xd416\n");
  POP_LO;
  fprintf(out, "  sta 0xd415\n");

  return 0;
}

int C64::c64_sid_filter_resonance(/* value */) { POKE(0xd417); return 0; }
int C64::c64_sid_volume(/* value */) { POKE(0xd418); return 0; }
int C64::c64_sid_voice3_oscillator(/* value */) { POKE(0xd41b); return 0; }
int C64::c64_sid_voice3_envelope(/* value */) { POKE(0xd41c); return 0; }

int C64::c64_sid_clear()
{
  fprintf(out, "; sid clear\n");
  fprintf(out, "  ldx #0x1c\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta 0xd400,x\n");
  fprintf(out, "  dex\n");
  fprintf(out, "  bpl #-8\n");

  return 0;
}

int C64::c64_vic_sprite0pos(/* x, y */)
{
  fprintf(out, "; sprite0pos\n");

  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta 0xd001\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 1\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI;
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #(1 << 0)\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO;
  fprintf(out, "  sta 0xd000\n");

  return 0;
}

int C64::c64_vic_sprite1pos(/* x, y */)
{
  fprintf(out, "; sprite1pos\n");

  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta 0xd003\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 2\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI;
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #2\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO;
  fprintf(out, "  sta 0xd002\n");

  return 0;
}

int C64::c64_vic_sprite2pos(/* x, y */)
{
  fprintf(out, "; sprite2pos\n");

  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta 0xd005\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 4\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI;
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #4\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO;
  fprintf(out, "  sta 0xd004\n");

  return 0;
}

int C64::c64_vic_sprite3pos(/* x, y */)
{
  fprintf(out, "; sprite3pos\n");

  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta 0xd007\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 8\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI;
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #8\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO;
  fprintf(out, "  sta 0xd006\n");

  return 0;
}

int C64::c64_vic_sprite4pos(/* x, y */)
{
  fprintf(out, "; sprite4pos\n");

  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta 0xd009\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 16\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI;
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #16\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO;
  fprintf(out, "  sta 0xd008\n");

  return 0;
}

int C64::c64_vic_sprite5pos(/* x, y */)
{
  fprintf(out, "; sprite5pos\n");

  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta 0xd00b\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 32\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI;
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #32\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO;
  fprintf(out, "  sta 0xd00a\n");

  return 0;
}

int C64::c64_vic_sprite6pos(/* x, y */)
{
  fprintf(out, "; sprite6pos\n");

  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta 0xd00d\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 64\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI;
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #64\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO;
  fprintf(out, "  sta 0xd00c\n");

  return 0;
}

int C64::c64_vic_sprite7pos(/* x, y */)
{
  fprintf(out, "; sprite7pos\n");

  // y
  POP_HI;
  POP_LO;
  fprintf(out, "  sta 0xd00f\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 128\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI;
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #128\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO;
  fprintf(out, "  sta 0xd00e\n");

  return 0;
}

int C64::c64_vic_write_control1(/* value */) { POKE(0xd011); return 0; }
int C64::c64_vic_read_control1() { PEEK(0xd011); return 0; }
int C64::c64_vic_wait_raster(/* line */)
{
  fprintf(out, "; wait_raster\n");
  POP_HI;
  POP_LO;
  fprintf(out, "cmp 0xd012\n");
  fprintf(out, "bne #-5\n");

  return 0;
}
int C64::c64_vic_sprite_enable(/* value */) { POKE(0xd015); return 0; }
int C64::c64_vic_write_control2(/* value */) { POKE(0xd016); return 0; }
int C64::c64_vic_read_control2() { PEEK(0xd016); return 0; }
int C64::c64_vic_sprite_expandy(/* value */) { POKE(0xd017); return 0; }
int C64::c64_vic_write_pointer(/* value */) { POKE(0xd018); return 0; }
int C64::c64_vic_read_pointer() { PEEK(0xd018); return 0; }
int C64::c64_vic_sprite_priority(/* value */) { POKE(0xd01b); return 0; }
int C64::c64_vic_sprite_multicolor_enable(/* value */) { POKE(0xd01c); return 0; }
int C64::c64_vic_sprite_expandx(/* value */) { POKE(0xd01d); return 0; }
int C64::c64_vic_sprite_collision() { PEEK(0xd01e); return 0; }
int C64::c64_vic_data_collision() { PEEK(0xd01f); return 0; }
int C64::c64_vic_border(/* value */) { POKE(0xd020); return 0; }
int C64::c64_vic_background(/* value */) { POKE(0xd021); return 0; }
int C64::c64_vic_background1(/* value */) { POKE(0xd022); return 0; }
int C64::c64_vic_background2(/* value */) { POKE(0xd023); return 0; }
int C64::c64_vic_background3(/* value */) { POKE(0xd024); return 0; }
int C64::c64_vic_sprite_multicolor0(/* value */) { POKE(0xd025); return 0; }
int C64::c64_vic_sprite_multicolor1(/* value */) { POKE(0xd026); return 0; }
int C64::c64_vic_sprite0color(/* value */) { POKE(0xd027); return 0; }
int C64::c64_vic_sprite1color(/* value */) { POKE(0xd028); return 0; }
int C64::c64_vic_sprite2color(/* value */) { POKE(0xd029); return 0; }
int C64::c64_vic_sprite3color(/* value */) { POKE(0xd02a); return 0; }
int C64::c64_vic_sprite4color(/* value */) { POKE(0xd02b); return 0; }
int C64::c64_vic_sprite5color(/* value */) { POKE(0xd02c); return 0; }
int C64::c64_vic_sprite6color(/* value */) { POKE(0xd02d); return 0; }
int C64::c64_vic_sprite7color(/* value */) { POKE(0xd02e); return 0; }

#if 0
void C64::close()
{
}
#endif
