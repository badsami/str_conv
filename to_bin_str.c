#include "to_bin_str.h"
#include "utilities.h"


// With a pre-computed bit count
void u32_sized_to_bin_str(schar8* num_str_start, u32 num, u32 bit_count)
{
  schar8* bit_dest = num_str_start + bit_count;
  do
  {
    bit_dest--;
    schar8 bit = num & 0b1;
    *bit_dest = '0' + bit;
    num >>= 1;
  }
  while (bit_dest > num_str_start);
}


void u64_sized_to_bin_str(schar8* num_str_start, u64 num, u32 bit_count)
{
  schar8* bit_dest = num_str_start + bit_count;
  do
  {
    bit_dest--;
    schar8 bit = num & 0b1;
    *bit_dest = '0' + bit;
    num >>= 1;
  }
  while (bit_dest > num_str_start);
}


void s32_sized_to_bin_str(schar8* num_str_start, s32 num, u32 bit_count)
{
  u32_sized_to_bin_str(num_str_start, num, bit_count);
}


void s64_sized_to_bin_str(schar8* num_str_start, s64 num, u32 bit_count)
{
  u64_sized_to_bin_str(num_str_start, num, bit_count);
}


void f32_sized_to_bin_str(schar8* num_str_start, f32 num, u32 bit_count)
{
  u32 num_bits = *(u32*)&num;
  u32_sized_to_bin_str(num_str_start, num_bits, bit_count);
}


// Without a pre-computed bit count
u32 u32_to_bin_str(schar8* num_str_start, u32 num)
{
  u32 bit_count = u32_bit_count(num);
  u32_sized_to_bin_str(num_str_start, num, bit_count);

  return bit_count;
}


u32 u64_to_bin_str(schar8* num_str_start, u64 num)
{
  u32 bit_count = u64_bit_count(num);
  u64_sized_to_bin_str(num_str_start, num, bit_count);
  
  return bit_count;
}


u32 s32_to_bin_str(schar8* num_str_start, s32 num)
{
  return u32_to_bin_str(num_str_start, num);
}


u32 s64_to_bin_str(schar8* num_str_start, s64 num)
{
  return u64_to_bin_str(num_str_start, num);
}


u32 f32_to_bin_str(schar8* num_str_start, f32 num)
{
  u32 num_bits     = *(u32*)&num;
  u32 num_str_size = u32_to_bin_str(num_str_start, num_bits);

  return num_str_size;
}
