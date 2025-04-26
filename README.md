# str_conv
Integers and 32-bit floating-point numbers conversion functions to and from strings of binary, decimal and hexadecimal characters.

```C
#include "to_dec_str.h"
#include <Windows.h>

void print_numbers(HANDLE output, schar8* output_buffer, f32* numbers, u32 numbers_count)
{
  schar8* begin = output_buffer;
  schar8* end   = output_buffer;
  
  for (u32 i = 0; i < numbers_count; i++)
  {
    f32 num          = numbers[i];
    u32 num_str_size = f32_to_dec_str(end, num);
    
    end[num_str_size] = '\n';
    end += num_str_size + 1u;
  }

  u32 char_written_count = (u32)(end - begin);
  WriteFile(output, output_buffer, char_written_count, 0, 0);
}
```

You can compile this example yourself by running [`build.bat`](build.bat) (requires Visual Studio
Build Tools or the Native Desktop workload):
```batch
build && build run
```


## Features
- Signed and unsigned integers up to 64-bit to binary, decimal and hexadecimal ASCII strings, including from a pre-determined count of numerals
  - Capital letters are used when writing hexadecimal strings
- 32-bit floating point numbers to binary, decimal and hexadecimal ASCII strings, including from a pre-determined count of numerals or a pre-determined minimum integer part size and fixed fractional part size, with a few particularities:
  - Values outside of [-2^32 + 1, 2^32 - 1] are converted to string `-big` or `big`, depending on their sign (see rational in `f32_number_to_dec_str` comments, in [to_dec_str.c](https://github.com/badsami/str_conv/blob/main/to_dec_str.c#L218))
  - `"-qnan"`, `"qnan"`, `"-snan"`, `"snan"`, `"-inf"` or `"inf"` may be output for matching non-number values
  - `-0` will be converted to `"0"`
  - By default, 6 digits past the period are written to the output string. This can be increased up to 9 (see `F32_DEC_FRAC_DEFAULT_STR_SIZE` and `F32_DEC_FRAC_MULT` in [types_max_str_size.h](types_max_str_size.h))
  - When passing sizes for the integer and fractional parts, the integer part is padded withs 0 to the left if necessary, and the fractional part's will be truncated to the maximum allowed size of 9 if it exceeds this value (see `F32_DEC_FRAC_MAX_STR_SIZE` in [types_max_str_size.h](types_max_str_size.h))
  - Values are written in full. Scientific notation and other notations are not supported

- Binary, decimal and hexadecimal ASCII strings to signed and unsigned integers up to 64-bit, including from a pre-determined string size
- Binary, decimal and hexadecimal ASCII strings to 32-bit floating point numbers, including from a pre-determined string size, with the following particularities:
  - Values exceeding the bounds of the 32-bit floating point type are not handled
  - Strings `-big` and `big` are converted to -2^32 and 2^32, respectively
  - Strings `-qnan`, `qnan`, `-snan`, `snan`, `-inf` and `inf` are converted to their matching bits representation. For `+/- qnan`, the leftmost, highest mantissa bit is set, while for `-/+ snan` and `-/+ inf`, the mantissa is all zeros
  - `"-0"` will be converted to `0`
  - Numbers must start with either `-` or a numeral, but not with `.`
  - Scientific notation and other notations are not supported

- A few utilities for counting numerals in a number and qualify a single character
- No allocation, only user-provided memory buffers
- No C runtime, no C standard library


## Supported targets
- Windows XP and above
- 64-bit and 32-bit x86 CPU architectures
- MSVC compiler


## Repository files
- `[from|to]_<base_abbreviation>_str.[h|c]`: conversion functions
- [`utilities.h`](utilities.h) / [`utilities.c`](utilities.c): numerals counting and identification functions
- [`types.h`](types.h): custom typedefs, for personal convenience
- [`types_max_str_size.h`](types_max_str_size.h): macros defining the maximum amount of characters written when converting to strings, to make it easier to initialise a destination buffer of sufficient size on the stack for instance
- [`isa.h`](isa.h): macros to identify which compiler is being used and for which CPU instruction set code is being compiled for


## License
The code in this repository is released in the public domain. You can use it with no constraint.
