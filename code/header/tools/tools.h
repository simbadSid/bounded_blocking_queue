/**
 * @def tools
 * @details Contains all the snippets of code that are shared in the project.
 * @author Riyane
 * @date 20/05/2022
 */


#ifndef TOOLS_H
#define TOOLS_H

#include <cstdarg>
#include <cstdio>


#ifdef LOG
    void log(const char *format, ...);
#else
    #define log(...) ((void)0)
#endif

void log_error(const char *format, ...);

                                                    \
#endif //TOOLS_H
