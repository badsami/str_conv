#include "from_hex_str.h"
#include "utilities.h"


// Table
static const u8 hex_char_to_value[] =
{
  0,   1,  2,  3,  4,  5,  6,  7,  8,  9,
  0,   0,  0,  0,  0,  0,  0, 10, 11, 12,
  13, 14, 15,  0,  0,  0,  0,  0,  0,  0,
  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,   0,  0,  0,  0,  0,  0,  0,  0, 10,
  11, 12, 13, 14, 15
};


// With a predetermined string size
u32 u32_from_sized_hex_str(const schar8* num_str_start, u32 num_str_size)
{
  const schar8* const num_str_start_end = num_str_start + num_str_size;

  u32 num = 0u;
  while (num_str_start < num_str_start_end)
  {
    num <<= 4;
    num |= hex_char_to_value[*num_str_start - '0'];
    num_str_start++;
  }

  return num;
}


u64 u64_from_sized_hex_str(const schar8* num_str_start, u32 num_str_size)
{
  const schar8* const num_str_start_end = num_str_start + num_str_size;
  
  u64 num = 0u;
  while (num_str_start < num_str_start_end)
  {
    num <<= 4;
    num |= hex_char_to_value[*num_str_start - '0'];
    num_str_start++;
  }

  return num;
}


s32 s32_from_sized_hex_str(const schar8* num_str_start, u32 num_str_size)
{
  u32 num = u32_from_sized_hex_str(num_str_start, num_str_size);

  return (s32)num;
}


s64 s64_from_sized_hex_str(const schar8* num_str_start, u32 num_str_size)
{
  u64 num = u64_from_sized_hex_str(num_str_start, num_str_size);

  return (s64)num;
}


f32 f32_from_sized_hex_str(const schar8* num_str_start, u32 num_str_size)
{
  u32 num_bits = u32_from_sized_hex_str(num_str_start, num_str_size);
  
  return *(f32*)&num_bits;
}


// Without a predetermined string size
u32 u32_from_hex_str(const schar8* num_str_start)
{
  u32 num = 0u;
  while (is_hexadecimal(*num_str_start))
  {
    num <<= 4;
    num |= hex_char_to_value[*num_str_start - '0'];
    num_str_start++;
  }

  return num;
}


u64 u64_from_hex_str(const schar8* num_str_start)
{
  u64 num = 0u;
  while (is_hexadecimal(*num_str_start))
  {
    num <<= 4;
    num |= hex_char_to_value[*num_str_start - '0'];
    num_str_start++;
  }

  return num;
}


s32 s32_from_hex_str(const schar8* num_str_start)
{
  u32 num = u32_from_hex_str(num_str_start);
  
  return (s32)num;
}


s64 s64_from_hex_str(const schar8* num_str_start)
{
  u64 num = u64_from_hex_str(num_str_start);
  
  return (s64)num;
}


f32 f32_from_hex_str(const schar8* num_str_start)
{
  u32 num_bits = u32_from_hex_str(num_str_start);
  
  return *(f32*)&num_bits;
}
