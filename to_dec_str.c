#include "to_dec_str.h"
#include "utilities.h"
#include "types_max_str_size.h"

static const f32 f32_frac_size_to_mul[F32_DEC_FRAC_MAX_STR_SIZE + 1] =
{
  1.f,
  10.f,
  100.f,
  1000.f,
  10000.f,
  100000.f,
  1000000.f,
  10000000.f,
  100000000.f,
  1000000000.f
};


// With a pre-computed digit count
void u32_sized_to_dec_str(schar8* num_str_start, u32 num, u32 digit_count)
{
  schar8* digit_dest = num_str_start + digit_count;
  do
  {
    digit_dest--;

    u32    quotient    = num / 10u;
    schar8 right_digit = (schar8)(num - quotient * 10u);

    *digit_dest = '0' + right_digit;
    num = quotient;
  }
  while (digit_dest > num_str_start);
}


void u64_sized_to_dec_str(schar8* num_str_start, u64 num, u32 digit_count)
{
  schar8* digit_dest = num_str_start + digit_count;
  do
  {
    digit_dest--;

    u64    quotient    = num / 10Lu;
    schar8 right_digit = (schar8)(num - quotient * 10Lu);

    *digit_dest = '0' + right_digit;
    num = quotient;
  }
  while (digit_dest > num_str_start);
}


u32 s32_sized_to_dec_str(schar8* num_str_start, u32 is_neg, u32 abs_num, u32 abs_num_digit_count)
{
  // The negative sign is overwritten if it is unnecessary
  num_str_start[0] = '-';
  
  u32_sized_to_dec_str(num_str_start + is_neg, abs_num, abs_num_digit_count);

  return abs_num_digit_count + is_neg;
}


u32 s64_sized_to_dec_str(schar8* num_str_start, u32 is_neg, u64 abs_num, u32 abs_num_digit_count)
{
  // The negative sign is overwritten if it is unnecessary
  num_str_start[0] = '-';
  
  u64_sized_to_dec_str(num_str_start + is_neg, abs_num, abs_num_digit_count);

  return abs_num_digit_count + is_neg;
}


u32 f32_sized_to_dec_str(schar8* num_str_start, f32 num, u32 min_int_size, u32 frac_size)
{
  schar8* last_digit = num_str_start;  
  u32     is_neg     = num < 0.f;
  
  // The negative sign is overwritten if it is unnecessary
  last_digit[0] = '-';
  last_digit += is_neg;

  num = is_neg ? -num : num;

  if (num < 4294967296.f)
  {
    u32 num_int        = (u32)num;
    u32 int_digit_count = u32_digit_count(num_int);
        int_digit_count = (min_int_size < int_digit_count) ? int_digit_count : min_int_size;

    u32_sized_to_dec_str(last_digit, num_int, int_digit_count);

    last_digit += int_digit_count;
    
    if (num < 8388608.f)
    {
      f32 num_rounded = (f32)num_int;
      f32 num_frac    = num - num_rounded;
      if (num_frac >= 0.000001f)
      {
        *last_digit = '.';
        last_digit += 1u;

        // There can never be more fractional digits than the configured maximum allowed
        u32 num_frac_size = (frac_size < F32_DEC_FRAC_MAX_STR_SIZE) ?
                            frac_size : F32_DEC_FRAC_MAX_STR_SIZE;

        f32 num_frac_ext = num_frac * f32_frac_size_to_mul[num_frac_size];
        u32 num_frac_int = (u32)num_frac_ext;

        // See types_max_str_size.h
        u32_sized_to_dec_str(last_digit, num_frac_int, num_frac_size);
        last_digit += num_frac_size;
      }
    }
  }
  else
  {
    last_digit[0] = 'b';
    last_digit[1] = 'i';
    last_digit[2] = 'g';

    last_digit += 3u;
  }

  return (u32)(last_digit - num_str_start);
}


// Without a pre-computed digit count
u32 u32_to_dec_str(schar8* num_str_start, u32 num)
{
  u32 digit_count = u32_digit_count(num);
  u32_sized_to_dec_str(num_str_start, num, digit_count);
  return digit_count;
}


u32 u64_to_dec_str(schar8* num_str_start, u64 num)
{
  u32 digit_count = u64_digit_count(num);
  u64_sized_to_dec_str(num_str_start, num, digit_count);
  return digit_count;
}


u32 s32_to_dec_str(schar8* num_str_start, s32 num)
{
  num_str_start[0] = '-';
  
  // Branchless (is_neg ? -num : num)
  s32 is_neg      = num < 0;
  u32 pos_num     = (num ^ -is_neg) + is_neg;
  u32 digit_count = u32_digit_count(pos_num);
  s32_sized_to_dec_str(num_str_start, is_neg, pos_num, digit_count);
  
  return digit_count + is_neg;
}


u32 s64_to_dec_str(schar8* num_str_start, s64 num)
{
  num_str_start[0] = '-';
  
  // Branchless (is_neg ? -num : num)
  s32 is_neg      = num < 0;
  u64 pos_num     = (num ^ -is_neg) + is_neg;
  u32 digit_count = u64_digit_count(pos_num);
  s64_sized_to_dec_str(num_str_start, is_neg, pos_num, digit_count);
  
  return digit_count + is_neg;
}


u32 f32_nan_or_inf_str(schar8* num_str_start, f32 num)
{
  u32 num_bits = *(u32*)&num;
  
  // num is +infinity, -infinity, qnan, -qnan, snan or -snan
  // The negative sign is overwritten if it is unnecessary
  num_str_start[0] = '-';

  schar8* sentinel = num_str_start;
  sentinel += num_bits >> 31;
 
  const u32 MANTISSA_MASK = 0x007FFFFF;
  u32 mantissa_bits = num_bits & MANTISSA_MASK;
  if (mantissa_bits == 0)
  {
    sentinel[0] = 'i';
    sentinel[1] = 'n';
    sentinel[2] = 'f';
    sentinel += 3;
  }
  else
  {
    // A nan is a float with all the exponent bits set and at least one fraction bit set.
    // A quiet nan (qnan) is a nan with the leftmost, highest fraction bit set.
    // A signaling nan (qnan) is a nan with the leftmost, highest fraction bit clear.
    // 's' - 'q' is 2, which is a multiple of 2. We can use this to change 's' to a 'q' by
    // offsetting 's' to 'q' without ifs
    schar8 is_quiet_offset = (schar8)((num_bits & 0x00400000) >> 21);
    
    sentinel[0] = 's' - is_quiet_offset;
    sentinel[1] = 'n';
    sentinel[2] = 'a';
    sentinel[3] = 'n';
    sentinel += 4;
  }

  return (u32)(sentinel - num_str_start);
}


u32 f32_number_to_dec_str(schar8* num_str_start, f32 num)
{
  schar8* last_digit = num_str_start;  
  u32     is_neg     = num < 0.f;
  
  // The negative sign is overwritten if it is unnecessary
  last_digit[0] = '-';
  last_digit += is_neg;

  num = is_neg ? -num : num;

  // Absolute values equal or greater to 8 388 608 are likely better represented as a s32 or s64
  // than as a 32-bit floating-point value (thereafter referred to as "f32"), for two reasons:
  // 
  // - Range of values: f32 values can be as large as +/- 3.4 x 10^38. I believe such large values
  // are never needed in video games. If they are, s32 can represent values up to +/- 2.15 x 10^9
  // and s64 can represent values up to +/- 9.22 x 10^18 instead, which I think would cover any
  // game programmer's needs
  // 
  // - Precision: starting at 8 388 608 and beyond, f32 values cannot have a fractional part.
  // Starting at 16 777 216 and beyond, adding 1 to a f32 value doesn't change it. The fractional
  // part is not very significant at this magnitude, but s32 and s64 retain a precision of 1 on the
  // full range of values they cover
  //
  // Nonetheless, 8 388 608 needs 23 bits to be represented, which would require a u32. So the full
  // range of the u32 type (+/- [0; 4 294 967 296]) may as well be supported, adding an extra
  // margin
  if (num < 4294967296.f)
  {
    u32 num_int        = (u32)num;
    u32 int_digit_size = u32_to_dec_str(last_digit, num_int);

    last_digit += int_digit_size;
    
    if (num < 8388608.f)
    {
      // The same way values as large as +/- 3.4 x 10^38 are, in my view, never needed, I believe 
      // precision under 0.000001 is also never needed. 0.000001 is precise enough:
      // - if 1.0 = 1 meter,  0.000001 = 1 micrometer
      // - if 1.0 = 1 radian, 0.000001 = 1 / 6 283 185th of a circle = 0.000057 degrees
      f32 num_rounded = (f32)num_int;
      f32 num_frac    = num - num_rounded;
      if (num_frac >= 0.000001f)
      {
        *last_digit = '.';
        last_digit += 1u;

        // Fun fact: for floating-point values with exponent n (n < 23), the maximum count of decimal
        // fractional digit is 23 - n. Using the unbiased exponent u, this is the same as 150 - u.
        //
        //   u32 num_bits             = *(u32*)&num;
        //   s8  unbiased_exp         = (s8)((num_bits & 0x7F800000) >> 23);
        //   u8  max_frac_digit_count = 150u - unbiased_exp;

        // See types_max_str_size.h
        f32 num_frac_ext = num_frac * F32_DEC_FRAC_MULT;
        u32 num_frac_int = (u32)num_frac_ext;

        // See types_max_str_size.h
        u32_sized_to_dec_str(last_digit, num_frac_int, F32_DEC_FRAC_DEFAULT_STR_SIZE);
        last_digit += F32_DEC_FRAC_DEFAULT_STR_SIZE;
      }
    }
  }
  else
  {
    last_digit[0] = 'b';
    last_digit[1] = 'i';
    last_digit[2] = 'g';

    last_digit += 3u;
  }

  return (u32)(last_digit - num_str_start);
}


u32 f32_to_dec_str(schar8* num_start_start, f32 num)
{
  u32 is_a_number = f32_is_a_number(num);
  if (is_a_number)
  {
    return f32_number_to_dec_str(num_start_start, num);
  }
  else
  {
    return f32_nan_or_inf_str(num_start_start, num);
  }
}
