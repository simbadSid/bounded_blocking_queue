#include "tools.h"



#ifdef LOG
void log(const char *format, ...)
{
//TODO ad date time in logger

    va_list ap;
    va_start (ap, format);
    vfprintf (stdout, format, ap);
    va_end (ap);
    fflush (stdout);
}
#endif

void log_error(const char *format, ...)
{
    fprintf(stderr, "***********************\n");

    va_list ap;
    va_start (ap, format);
    vfprintf (stderr, format, ap);
    va_end (ap);
    fflush (stderr);
}
