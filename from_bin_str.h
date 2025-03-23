// String to binary number conversion functions
#pragma once

#include "types.h"


// With a predetermined string size
u32 u32_from_sized_bin_str(const schar8* num_str_start, u32 num_str_size);
u64 u64_from_sized_bin_str(const schar8* num_str_start, u32 num_str_size);
s32 s32_from_sized_bin_str(const schar8* num_str_start, u32 num_str_size);
s64 s64_from_sized_bin_str(const schar8* num_str_start, u32 num_str_size);
f32 f32_from_sized_bin_str(const schar8* num_str_start, u32 num_str_size);


// Without a predetermined string size
u32 u32_from_bin_str(const schar8* num_str_start);
u64 u64_from_bin_str(const schar8* num_str_start);
s32 s32_from_bin_str(const schar8* num_str_start);
s64 s64_from_bin_str(const schar8* num_str_start);
f32 f32_from_bin_str(const schar8* num_str_start);
