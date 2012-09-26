

#include <assert.h>

#ifdef __cplusplus
extern "C"
#endif
char asprintf();

int main() {
#if defined (__stub_asprintf) || defined (__stub___asprintf)
  fail fail fail
#else
  asprintf();
#endif

  return 0;
}
