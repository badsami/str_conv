// Number to decimal string conversion functions
#pragma once

#include "types.h"


// With predetermined digit counts
void u32_sized_to_dec_str(schar8* num_str_start, u32 num, u32 digit_count);
void u64_sized_to_dec_str(schar8* num_str_start, u64 num, u32 digit_count);
u32  s32_sized_to_dec_str(schar8* num_str_start, u32 is_neg, u32 abs_num, u32 abs_num_digit_count);
u32  s64_sized_to_dec_str(schar8* num_str_start, u32 is_neg, u64 abs_num, u32 abs_num_digit_count);
u32  f32_sized_to_dec_str(schar8* num_str_start, f32 num, u32 min_int_size, u32 frac_size);


// Without predetermined digit counts
u32 u32_to_dec_str(schar8* num_str_start, u32 num);
u32 u64_to_dec_str(schar8* num_str_start, u64 num);
u32 s32_to_dec_str(schar8* num_str_start, s32 num);
u32 s64_to_dec_str(schar8* num_str_start, s64 num);
u32 f32_nan_or_inf_str(schar8* num_str_start, f32 num);
u32 f32_number_to_dec_str(schar8* num_str_start, f32 num);
u32 f32_to_dec_str(schar8* num_str_start, f32 num);

#define to_dec_str(buffer, num) \
  _Generic((num),               \
           u32: u32_to_dec_str, \
           u64: u64_to_dec_str, \
           s32: s32_to_dec_str, \
           s64: s64_to_dec_str, \
           f32: f32_to_dec_str  \
          )(buffer, num)
