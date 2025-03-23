#include "utilities.h"
#include "isa.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Intrinsics
// Declare the specific intrinsics used below as extern, rather than including the 1025-line-long
// intrin.h header 
extern u8 _BitScanReverse(u32* msb_idx, u32 mask);

#if defined(ISA_x64)
  extern u8 _BitScanReverse64(u32* msb_idx, u64 mask);
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Intrisics wrapper
u32 bsr32(u32 num)
{
  u32 msb_idx;
  _BitScanReverse(&msb_idx, num);
  return msb_idx;
}


u32 bsr64(u64 num)
{
#if defined(ISA_x64)
  u32 msb_idx;
  _BitScanReverse64(&msb_idx, num);
  return msb_idx;
#elif defined(ISA_x86)
  // Assumming little endian order
  u32* halves            = (u32*)&num;
  u32  relevant_half_idx = halves[1] != 0;
  u32  relevant_half     = halves[relevant_half_idx];
  u32  msb_idx           = bsr32(relevant_half);
  u32  msb_idx_offset    = relevant_half_idx << 5;
  
  return msb_idx + msb_idx_offset;
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Numerals count in a number
u32 u32_digit_count(u32 num)
{
  // https://commaok.xyz/post/lookup_tables/
  static const u64 table[32] =
  {
    4294967296,                                         // (1  << 32)
    8589934582,  8589934582,  8589934582,               // (2  << 32) - 10
    12884901788, 12884901788, 12884901788,              // (3  << 32) - 100
    17179868184, 17179868184, 17179868184,              // (4  << 32) - 1000
    21474826480, 21474826480, 21474826480, 21474826480, // (5  << 32) - 10000
    25769703776, 25769703776, 25769703776,              // (6  << 32) - 100000
    30063771072, 30063771072, 30063771072,              // (7  << 32) - 1000000
    34349738368, 34349738368, 34349738368, 34349738368, // (8  << 32) - 10000000
    38554705664, 38554705664, 38554705664,              // (9  << 32) - 100000000
    41949672960, 41949672960, 41949672960,              // (10 << 32) - 1000000000
    42949672960, 42949672960                            // (10 << 32)
  };

  u32 msb_idx = bsr32(num | 0b1);
  return (num + table[msb_idx]) >> 32;
}


u32 u64_digit_count(u64 num)
{
  static const u32 msb_to_max_digit_count[64] =
  {
    1, 1, 1,
    2, 2, 2,
    3, 3, 3,
    4, 4, 4, 4,
    5, 5, 5,
    6, 6, 6,
    7, 7, 7, 7,
    8, 8, 8,
    9, 9, 9,
    10, 10, 10, 10,
    11, 11, 11,
    12, 12, 12,
    13, 13, 13, 13,
    14, 14, 14,
    15, 15, 15,
    16, 16, 16, 16,
    17, 17, 17,
    18, 18, 18,
    19, 19, 19, 19,
    20
  };
  
  static const u64 thresholds[20] =
  {
    0llu,
    10llu,
    100llu,
    1000llu,
    10000llu,
    100000llu,
    1000000llu,
    10000000llu,
    100000000llu,
    1000000000llu,
    10000000000llu,
    100000000000llu,
    1000000000000llu,
    10000000000000llu,
    100000000000000llu,
    1000000000000000llu,
    10000000000000000llu,
    100000000000000000llu,
    1000000000000000000llu,
    10000000000000000000llu
  };

  u32 msb_idx = bsr64(num | 0b1);

  u32 digit_count = msb_to_max_digit_count[msb_idx];
  digit_count -= (num < thresholds[digit_count - 1u]);
  return digit_count;
}


u32 u32_bit_count(u32 num)
{
  return bsr32(num | 0b1) + 1u;
}


u32 u64_bit_count(u64 num)
{
  return bsr64(num | 0b1) + 1u;
}


u32 u32_nibble_count(u32 num)
{
  u32 msb_idx      = bsr32(num | 0b1);
  u32 nibble_count = 1u + (msb_idx >> 2);

  return nibble_count;
}


u32 u64_nibble_count(u64 num)
{
  u32 msb_idx      = bsr64(num | 0b1);
  u32 nibble_count = 1u + (msb_idx >> 2);
  
  return nibble_count;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Floating-point specific values
u32 f32_is_a_number(f32 num)
{
  const u32 EXPONENT_ALL_ONE = 0x7F800000;
  
  u32 num_bits = *(u32*)&num;
  
  return (num_bits & EXPONENT_ALL_ONE) != EXPONENT_ALL_ONE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Character base check
u32 is_decimal(schar8 c)
{
  return (u8)(c - '0') <= 9u;
}


u32 is_binary(schar8 c)
{
  return (u8)(c - '0') <= 1u;
}


u32 is_hexadecimal(schar8 c)
{
  // A hexadecimal number is any character in intervals:
  // - ['0', '9'] = [48, 57]
  // - ['A', 'F'] = [65, 70]
  // - ['a', 'f'] = [97, 102]
  //
  // So relevant characters are in [48, 102], but not all
  // values in this interval match hexadecimal characters.
  //
  // Because the interval's range is 55 (102 - 48 + 1), its
  // characters can be mapped to the bits of a u64, where
  // the index of a bit represents a character of the
  // interval, and the value of the bit represents whereas
  // it matches a hexadecimal character.
  //
  // To make it so bit index 0 matches character 48 ('0'),
  // 48 ('0') is subtracted from the input character. With a
  // bit shift, the bit value at the matching bit index
  // returns whether the input is an hexadecimal character.
  //
  // Input values outside the hexadecimal interval are
  // handled by using an unsigned subtraction to get a
  // positive bit index in a way the C standard defines.
  // This way, inputs in [0, 47] (under '0') yield
  // [208, 255], and inputs in [103, 127] (above 'f')
  // yield [55, 79]. Values in [55, 63] are mapped to 0
  // bits in the u64. Values in [64, 79] and [103, 127]
  // will generate bit shifts that are undefined behaviour
  // in theory, but well-handled in practice by CPUs:
// #if defined(ISA_x64)
//   const u64 hex_char_bit_mask = 0b1111110000000000000000000000000011111100000001111111111;
//   u8 shift = c - '0';
//   return (hex_char_bit_mask >> shift) & 0b1;
// #else
//   const u32 hex_char_bit_mask[2] = {0b11111100000000000000000, 0b00000000011111100000001111111111};
//   u8 shift = c - '0';
//   u8 mask_idx = (shift > 31u);
//   shift -= mask_idx * 31;
//   return (hex_char_bit_mask[mask_idx] >> shift) & 0b1;
// #endif
  return ((u8)(c - '0') <= 9u) ||
         ((u8)(c - 'A') <= 5u) ||
         ((u8)(c - 'a') <= 5u);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Strings
u32 str_is_f32_decimal_number(const schar8* num_str_start)
{
  schar8 first_char          = num_str_start[0];
  u32    is_neg              = (first_char == '-');
  schar8 first_non_sign_char = num_str_start[is_neg];
  
  return is_decimal(first_non_sign_char);
}
