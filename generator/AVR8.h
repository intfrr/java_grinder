/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 * AVR8 written by Joe Davisson
 *
 */

#ifndef _AVR8_H
#define _AVR8_H

#include "Generator.h"

enum
{
  ATTINY13,
  ATTINY85,
  ATTINY84,
  ATTINY2313,
  ATMEGA328,
  ATMEGA328P,
};

class AVR8 : public Generator
{
public:
  AVR8(uint8_t chip_type);
  virtual ~AVR8();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int insert_static_field_define(const char *name, const char *type, int index);
  virtual int init_heap(int field_count);
  virtual int field_init_int(char *name, int index, int value);
  virtual int field_init_ref(char *name, int index);
  virtual void method_start(int local_count, int max_stack, int param_count, const char *name);
  virtual void method_end(int local_count);
  virtual int push_local_var_int(int index);
  virtual int push_local_var_ref(int index);
  virtual int push_ref_static(const char *name, int index);
  virtual int push_fake();
  virtual int push_int(int32_t n);
  virtual int push_long(int64_t n);
  virtual int push_float(float f);
  virtual int push_double(double f);
  virtual int push_ref(char *name);
  virtual int pop_local_var_int(int index);
  virtual int pop_local_var_ref(int index);
  virtual int pop();
  virtual int dup();
  virtual int dup2();
  virtual int swap();
  virtual int add_integer();
  virtual int add_integer(int const_val);
  virtual int sub_integer();
  virtual int sub_integer(int const_val);
  virtual int mul_integer();
  virtual int mul_integer(int const_val);
  virtual int div_integer();
  virtual int div_integer(int const_val);
  virtual int mod_integer();
  virtual int mod_integer(int const_val);
  virtual int neg_integer();
  virtual int shift_left_integer();
  virtual int shift_left_integer(int const_val);
  virtual int shift_right_integer();
  virtual int shift_right_integer(int const_val);
  virtual int shift_right_uinteger();
  virtual int shift_right_uinteger(int const_val);
  virtual int and_integer();
  virtual int and_integer(int const_val);
  virtual int or_integer();
  virtual int or_integer(int const_val);
  virtual int xor_integer();
  virtual int xor_integer(int const_val);
  virtual int inc_integer(int index, int num);
  virtual int integer_to_byte();
  virtual int integer_to_short();
  virtual int jump_cond(const char *label, int cond, int distance);
  virtual int jump_cond_integer(const char *label, int cond, int distance);
  virtual int ternary(int cond, int value_true, int value_false);
  virtual int ternary(int cond, int compare, int value_true, int value_false);
  virtual int return_local(int index, int local_count);
  virtual int return_integer(int local_count);
  virtual int return_void(int local_count);
  virtual int jump(const char *name, int distance);
  virtual int call(const char *name);
  virtual int invoke_static_method(const char *name, int params, int is_void);
  virtual int put_static(const char *name, int index);
  virtual int get_static(const char *name, int index);
  virtual int brk();
  virtual int new_array(uint8_t type);
  virtual int insert_array(const char *name, int32_t *data, int len, uint8_t type);
  virtual int insert_string(const char *name, uint8_t *bytes, int len);
  virtual int push_array_length();
  virtual int push_array_length(const char *name, int field_id);
  virtual int array_read_byte();
  virtual int array_read_short();
  virtual int array_read_int();
  virtual int array_read_byte(const char *name, int field_id);
  virtual int array_read_short(const char *name, int field_id);
  virtual int array_read_int(const char *name, int field_id);
  virtual int array_write_byte();
  virtual int array_write_short();
  virtual int array_write_int();
  virtual int array_write_byte(const char *name, int field_id);
  virtual int array_write_short(const char *name, int field_id);
  virtual int array_write_int(const char *name, int field_id);
  //virtual void close();
  virtual int get_values_from_stack(int num);

  // Memory API
  virtual int memory_read8_I();
  virtual int memory_write8_IB();
  virtual int memory_read16_I();
  virtual int memory_write16_IS();

  // GPIO functions
  virtual int ioport_setPinsAsInput_I(int port);
  virtual int ioport_setPinsAsInput_I(int port, int const_val);
  virtual int ioport_setPinsAsOutput_I(int port);
  virtual int ioport_setPinsAsOutput_I(int port, int const_val);
  virtual int ioport_setPinsValue_I(int port);
  virtual int ioport_setPinsValue_I(int port, int const_val);
  virtual int ioport_setPinsHigh_I(int port);
  virtual int ioport_setPinsLow_I(int port);
  virtual int ioport_setPinAsOutput_I(int port);
  virtual int ioport_setPinAsInput_I(int port);
  virtual int ioport_setPinHigh_I(int port);
  virtual int ioport_setPinHigh_I(int port, int const_val);
  virtual int ioport_setPinLow_I(int port);
  virtual int ioport_setPinLow_I(int port, int const_val);
  virtual int ioport_isPinInputHigh_I(int port);
  virtual int ioport_getPortInputValue(int port);
  //virtual int ioport_setPortOutputValue_I(int port);

  // ADC Functions
  virtual int adc_enable();
  virtual int adc_disable();
  virtual int adc_setChannel_I();
  virtual int adc_setChannel_I(int channel);
  virtual int adc_read();

  // UART functions
  virtual int uart_init_I(int port);
  virtual int uart_init_I(int port, int baud_rate);
  virtual int uart_send_I(int port);
  virtual int uart_read(int port);
  virtual int uart_isDataAvailable(int port);
  virtual int uart_isSendReady(int port);

protected:
  int stack;
  bool is_main:1;
  const char *include_file;
  bool need_farjump:1;
  bool need_memory_mapped_adc:1;
  bool need_swap:1;
  bool need_add_integer:1;
  bool need_sub_integer:1;
  bool need_mul_integer:1;
  bool need_div_integer:1;
  bool need_mod_integer:1;
  bool need_neg_integer:1;
  bool need_shift_left_integer:1;
  bool need_shift_right_integer:1;
  bool need_shift_right_uinteger:1;
  bool need_and_integer:1;
  bool need_or_integer:1;
  bool need_xor_integer:1;
  bool need_inc_integer:1;
  bool need_integer_to_byte:1;
  bool need_jump_cond:1;
  bool need_jump_cond_integer:1;
  bool need_push_local_var_int:1;
  bool need_pop_local_var_int:1;
  bool need_dup:1;
  bool need_push_array_length:1;
  bool need_push_array_length2:1;
  bool need_array_byte_support:1;
  bool need_array_int_support:1;
  bool need_get_values_from_stack:1;

  void insert_swap();
  void insert_add_integer();
  void insert_sub_integer();
  void insert_mul_integer();
  void insert_div_integer();
  void insert_mod_integer();
  void insert_neg_integer();
  void insert_shift_left_integer();
  void insert_shift_right_integer();
  void insert_shift_right_uinteger();
  void insert_and_integer();
  void insert_or_integer();
  void insert_xor_integer();
  void insert_inc_integer();
  void insert_integer_to_byte();
  void insert_jump_cond();
  void insert_jump_cond_integer();
  void insert_push_local_var_int();
  void insert_pop_local_var_int();
  void insert_dup();
  void insert_push_array_length();
  void insert_push_array_length2();
  void insert_array_byte_support();
  void insert_array_int_support();
  void insert_get_values_from_stack();
};

#endif

