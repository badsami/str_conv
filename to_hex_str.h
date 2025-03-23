// Number to hexdecimal string conversion functions
#pragma once

#include "types.h"


// With a predetermined nibble count
void u32_sized_to_hex_str(schar8* num_str_start, u32 num, u32 nibble_count);
void u64_sized_to_hex_str(schar8* num_str_start, u64 num, u32 nibble_count);
void s32_sized_to_hex_str(schar8* num_str_start, s32 num, u32 nibble_count);
void s64_sized_to_hex_str(schar8* num_str_start, s64 num, u32 nibble_count);
void f32_sized_to_hex_str(schar8* num_str_start, f32 num, u32 nibble_count);

#define sized_to_hex_str(num_str_start, num, nibble_count) \
  _Generic((num),                                          \
           u32: u32_sized_to_hex_str,                      \
           u64: u64_sized_to_hex_str,                      \
           s32: s32_sized_to_hex_str,                      \
           s64: s64_sized_to_hex_str,                      \
           f32: f32_sized_to_hex_str                       \
          )(num_str_start, num, nibble_count)


// Without a predetermined nibble count
u32 u32_to_hex_str(schar8* num_str_start, u32 num);
u32 u64_to_hex_str(schar8* num_str_start, u64 num);
u32 s32_to_hex_str(schar8* num_str_start, s32 num);
u32 s64_to_hex_str(schar8* num_str_start, s64 num);
u32 f32_to_hex_str(schar8* num_str_start, f32 num);

#define to_hex_str(num_str_start, num) \
  _Generic((num),                      \
           u32: u32_to_hex_str,        \
           u64: u64_to_hex_str,        \
           s32: s32_to_hex_str,        \
           s64: s64_to_hex_str,        \
           f32: f32_to_hex_str         \
          )(num_str_start, num)
