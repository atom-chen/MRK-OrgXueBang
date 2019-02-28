#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif
    int t_sprintf(char *out, const char *format, va_list args );
    int t_snprintf( char *buf, unsigned int count, const char *format, va_list args  );
#ifdef __cplusplus
}
#endif
