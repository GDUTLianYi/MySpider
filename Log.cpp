//
// Created by lianyi on 18-2-28.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Log.h"
/**************************
 * Error-handling functions
 **************************/
/* $begin errorfuns */
/* $begin unixerror */
 void Log::unix_error(const char *msg) /* unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    //exit(0);
}
/* $end unixerror */

static void posix_error(const int code, char *msg) /* posix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    //exit(0);
}

static void dns_error(const char *msg) /* dns-style error */
{
    fprintf(stderr, "%s: \n", msg);
   // exit(0);
}

static void app_error(const char *msg) /* application error */
{
    fprintf(stderr, "%s\n", msg);
   // exit(0);
}
/* $end errorfuns */