#include "to_hex_str.h"
#include "utilities.h"


// With a pre-computed digit count
static const schar8 hex_digits[] = "0123456789ABCDEF";

void u32_sized_to_hex_str(schar8* num_str_start, u32 num, u32 nibble_count)
{
  schar8* nimble_dest = num_str_start + nibble_count;
  do
  {
    nimble_dest--;
    u32 nibble_idx = num & 0xF;
    *nimble_dest = hex_digits[nibble_idx];
    num >>= 4;
  }
  while (nimble_dest > num_str_start);
}


void u64_sized_to_hex_str(schar8* num_str_start, u64 num, u32 nibble_count)
{
  schar8* nimble_dest = num_str_start + nibble_count;
  do
  {
    nimble_dest--;
    u64 nibble_idx = num & 0xF;
    *nimble_dest = hex_digits[nibble_idx];
    num >>= 4;
  }
  while (nimble_dest > num_str_start);
}


void s32_sized_to_hex_str(schar8* num_str_start, s32 num, u32 nibble_count)
{
  u32_sized_to_hex_str(num_str_start, num, nibble_count);
}


void s64_sized_to_hex_str(schar8* num_str_start, s64 num, u32 nibble_count)
{
  u64_sized_to_hex_str(num_str_start, num, nibble_count);
}


void f32_sized_to_hex_str(schar8* num_str_start, f32 num, u32 nibble_count)
{
  u32 num_nibbles = *(u32*)&num;
  u32_sized_to_hex_str(num_str_start, num_nibbles, nibble_count);
}


// Without a pre-computed digit count
u32 u32_to_hex_str(schar8* num_str_start, u32 num)
{
  u32 nibble_count = u32_nibble_count(num);
  u32_sized_to_hex_str(num_str_start, num, nibble_count);
  
  return nibble_count;
}


u32 u64_to_hex_str(schar8* num_str_start, u64 num)
{
  u32 nibble_count = u64_nibble_count(num);
  u64_sized_to_hex_str(num_str_start, num, nibble_count);
  
  return nibble_count;
}


u32 s32_to_hex_str(schar8* num_str_start, s32 num)
{
  return u32_to_hex_str(num_str_start, num);
}


u32 s64_to_hex_str(schar8* num_str_start, s64 num)
{
  return u64_to_hex_str(num_str_start, num);
}


u32 f32_to_hex_str(schar8* num_str_start, f32 num)
{
  u32 num_as_u32 = *(u32*)&num;
  
  return u32_to_hex_str(num_str_start, num_as_u32);
}
