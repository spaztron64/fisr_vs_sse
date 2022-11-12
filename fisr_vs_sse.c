// WARNING: ugly and messy experimental code that never got finished ahead!
// Read at your own discretion.

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <xmmintrin.h>

#define NOINLINE __attribute__((noinline, noclone))

    NOINLINE float Standard_InvSqrt(float number) {
  asm("");
  float squareRoot = sqrt(number);
  return 1.0f / squareRoot;
}

NOINLINE float Fast_InvSqrt(float number) {
  asm("");
  long i;
  float x2, y;
  const float threehalfs = 1.5f;

  x2 = number * 0.5f;
  y = number;
  i = *(long *)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float *)&i;
  y = y * (threehalfs - (x2 * y * y));

  return y;
}

NOINLINE float SSE_InvSqrt(float number) {
  asm("");
  __m128 _srcRegister = _mm_set1_ps(number);
  __m128 _dstRegister = _mm_rsqrt_ps(_srcRegister);
  float array[4];
  _mm_storeu_ps(array, _dstRegister);
  return array[0];
}

int main() {
  uint64_t iterations = 100000000;

  clock_t start, end;
  double cpu_time_used = 0;

  start = clock();
  printf("Performing %lld iterations of fast inverse square root of 262144.\n "
         "In progress...\n",
         iterations);
  for (uint64_t i = 0; i < iterations; i++) {
    Standard_InvSqrt(262144);
  }
  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Standard Inverse Square Root: %f seconds (%.0f per second)\n",
         cpu_time_used, iterations / cpu_time_used);

  start = clock();
  for (uint64_t i = 0; i < iterations; i++) {
    Fast_InvSqrt(262144);
  }

  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Fast Inverse Square Root:     %f seconds (%.0f per second)\n",
         cpu_time_used, iterations / cpu_time_used);

  start = clock();
  for (uint64_t i = 0; i < iterations; i++) {
    SSE_InvSqrt(262144);
  }
  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("SSE Inverse Square Root:      %f seconds (%.0f per second)\n",
         cpu_time_used, iterations / cpu_time_used);

  return 0;
}
