#include "to_dec_str.h"
#include "types_max_str_size.h"

#include <Windows.h>

s32 _fltused;

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

void mainCRTStartup()
{
  // Prepare the output to print to (console)
  AttachConsole(ATTACH_PARENT_PROCESS);
  HANDLE output = CreateFileA("CONOUT$",
                              GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              0,
                              OPEN_EXISTING,
                              0,
                              0);

  // Generate some demonstration values
  volatile f32 nzero = -0.f;
  f32          ninf  = 1.f / nzero;
  f32          mqnan = ninf / ninf;
  
  f32 numbers[8] =
  {
    0.f,
    nzero,
    ninf,
    mqnan,
    9876543210.f,
    -9876543210.f,
    3.1415826535f,
    123321.123321f
  };

  // 8 32-bit floating-point numbers + 8 newline characters
  schar8 buffer[(F32_MAX_DEC_STR_SIZE * 8) + 8];
  
  print_numbers(output, buffer, numbers, 8);

  // Flush and exit
  CloseHandle(output);
  FreeConsole();

  ExitProcess(0);
}
