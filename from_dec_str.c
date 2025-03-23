#include "from_dec_str.h"
#include "utilities.h"


static const f32 plus_and_minus_one[2] =
{
  1.f,
  -1.f
};


static const f32 digit_to_f32[10] =
{
  0.f,
  1.f,
  2.f,
  3.f,
  4.f,
  5.f,
  6.f,
  7.f,
  8.f,
  9.f
};


// With a predetermined string size
u32 u32_from_sized_dec_str(const schar8* num_str_start, u32 num_str_size)
{
  const schar8* const num_str_start_end = num_str_start + num_str_size;
  
  u32 num = 0u;
  while (num_str_start < num_str_start_end)
  {
    num *= 10u;
    num += *num_str_start - '0';
    num_str_start++;
  }

  return num;
}


u64 u64_from_sized_dec_str(const schar8* num_str_start, u32 num_str_size)
{
  const schar8* const num_str_start_end = num_str_start + num_str_size;
  
  u64 num = 0u;
  while (num_str_start < num_str_start_end)
  {
    num *= 10u;
    num += *num_str_start - '0';
    num_str_start++;
  }

  return num;
}


s32 s32_from_sized_dec_str(const schar8* num_str_start, u32 num_str_size)
{
  s32 is_neg = (*num_str_start == '-');
  num_str_start      += is_neg;
  num_str_size -= (u32)is_neg;
  u32 num = u32_from_sized_dec_str(num_str_start, num_str_size);
  
  return (num ^ -is_neg) + is_neg;
}


s64 s64_from_sized_dec_str(const schar8* num_str_start, u32 num_str_size)
{
  s64 is_neg = (*num_str_start == '-');
  num_str_start      += is_neg;
  num_str_size -= (u32)is_neg;
  u64 num = u64_from_sized_dec_str(num_str_start, num_str_size);
  
  return (num ^ -is_neg) + is_neg;
}


f32 f32_from_sized_dec_str(const schar8* num_str_start, u32 num_str_size)
{
  const schar8* const num_str_size_end = num_str_start + num_str_size;
  
  // Process the sign (which is skipped if there is none)
  u32 is_neg = (*num_str_start == '-');
  num_str_start += is_neg;
  
  // Get the integer part
  f32    num_int    = 0.f;
  schar8 digit_char = *num_str_start;
  
  while (is_decimal(digit_char))
  {
    num_int *= 10.f;

    schar8 digit_idx   = digit_char - '0';
    f32    digit_value = digit_to_f32[digit_idx];
    num_int += digit_value;
    
    num_str_start++;

    if (num_str_start >= num_str_size_end)
    {
      f32 sign = plus_and_minus_one[is_neg];
      return sign * num_int;
    }
    else
    {
      digit_char = *num_str_start;
    }
  }

  if (*num_str_start != '.')
  {
    return (f32)num_int;
  }

  // Skip the '.'
  num_str_start++;
  if (num_str_start >= num_str_size_end)
  {
    return num_int;
  }

  // Get the fractional part
  f32 num_frac = 0.f;
  f32 divider  = 1.f;

  digit_char = *num_str_start;
  while (is_decimal(digit_char))
  {
    num_frac *= 10.f;
    divider  *= 0.1f;

    schar8 digit_idx   = digit_char - '0';
    f32    digit_value = digit_to_f32[digit_idx];
    num_frac += digit_value;
    
    num_str_start++;

    if (num_str_start >= num_str_size_end)
    {
      break;
    }
    else
    {
      digit_char = *num_str_start;
    }
  }

  // Assemble the sign, integer and fractional part
  f32 sign = plus_and_minus_one[is_neg];
  f32 num  = sign * (num_int + (num_frac * divider));
  
  return num;
}


// Without a predetermined string size
u32 u32_from_dec_str(const schar8* num_str_start)
{
  u32 num = 0u;
  while (is_decimal(*num_str_start))
  {
    num *= 10u;
    num += *num_str_start - '0';
    num_str_start++;
  }

  return num;
}


u64 u64_from_dec_str(const schar8* num_str_start)
{
  u64 num = 0u;
  while (is_decimal(*num_str_start))
  {
    num *= 10u;
    num += *num_str_start - '0';
    num_str_start++;
  }

  return num;
}


s32 s32_from_dec_str(const schar8* num_str_start)
{
  s32 is_neg = (*num_str_start == '-');
  num_str_start += is_neg;
  u32 num = u32_from_dec_str(num_str_start);

  return (num ^ -is_neg) + is_neg;
}


s64 s64_from_dec_str(const schar8* num_str_start)
{
  s64 is_neg = (*num_str_start == '-');
  num_str_start += is_neg;
  u64 num = u64_from_dec_str(num_str_start);
  
  return (num ^ -is_neg) + is_neg;
}


// num_str_start is treated as inf, -inf, snan, -snan, qnan, -qnan, big or -big
// big represents a number whose value is greater or equal to 2^32
f32 f32_from_dec_non_number_str(const schar8* num_str_start)
{
  u32 is_neg = *num_str_start == '-';
  num_str_start += is_neg;

  if (*num_str_start == 'b')
  {
    // Values greater or equal to 2^32 (4294967296) are considered "big" values. Beyond being large
    // numbers, they are not useful to me. They have an imprecision greater or equal to 512, and
    // could be replaced with a s64 or u64 from 2^32 to (2^63 - 1) or (2^64 - 1) (respectively),
    // which would retain a precision of 1 across their range
    const f32 TWO_POW_32 = 4294967296.f;
    
    return plus_and_minus_one[is_neg] * TWO_POW_32;
  }
  else
  {
    const u32 ALL_1_EXPONENT = 0x7F800000;
    
    u32 sign_mask = is_neg << 31;
    u32 num_bits  = sign_mask | ALL_1_EXPONENT;
    
    if (*num_str_start == 'i')
    {
      return *(f32*)&num_bits;
    }
    else
    {
      // A nan is a float with all the exponent bits set and at least one fraction bit set.
      // A quiet nan (qnan) is a nan with the leftmost, highest mantissa bit set.
      // A signaling nan (qnan) is a nan with the leftmost, highest mantissa bit clear.
      const u32 TWENTY_TWO = 0b10110;
                                                           // quiet nan      | signaling nan
      u32 is_signaling    = num_str_start[0] == 's';       // = 0            | 1
      u32 nan_shift_shift = is_signaling << 2;             // = 0            | 4
      u32 nan_shift       = TWENTY_TWO >> nan_shift_shift; // = 0b10110 (22) | 0b00000 (0)
      u32 nan_mask        = 1 << nan_shift;                // = 1 << 22      | 1 << 0
      
      num_bits = num_bits | nan_mask;
      
      return *(f32*)&num_bits;
    }
  }
}


f32 f32_from_dec_number_str(const schar8* num_str_start)
{
  // Process the sign (which is skipped if there is none)
  u32 is_neg = (*num_str_start == '-');
  num_str_start += is_neg;
  
  // Get the integer part
  f32    num_int    = 0.f;
  schar8 digit_char = *num_str_start;
  
  while (is_decimal(digit_char))
  {
    num_int *= 10.f;

    schar8 digit_idx   = digit_char - '0';
    f32    digit_value = digit_to_f32[digit_idx];
    num_int += digit_value;
    
    num_str_start++;
    digit_char = *num_str_start;
  }

  if (*num_str_start != '.')
  {
    return (f32)num_int;
  }

  // Skip the '.'
  num_str_start++;

  // Get the fractional part
  f32 num_frac = 0.f;
  f32 divider  = 1.f;

  digit_char = *num_str_start;
  while (is_decimal(digit_char))
  {
    num_frac *= 10.f;
    divider  *= 0.1f;

    schar8 digit_idx   = digit_char - '0';
    f32    digit_value = digit_to_f32[digit_idx];
    num_frac += digit_value;
    
    num_str_start++;
    digit_char = *num_str_start;
  }

  // Assemble the sign, integer and fractional part
  f32 sign = plus_and_minus_one[is_neg];
  f32 num  = sign * (num_int + (num_frac * divider));
  
  return num;
}


f32 f32_from_dec_str(const schar8* num_str_start)
{
  u32 is_decimal = str_is_f32_decimal_number(num_str_start);
  if (is_decimal)
  {
    return f32_from_dec_number_str(num_str_start);
  }
  else
  {
    return f32_from_dec_non_number_str(num_str_start);
  }
}
