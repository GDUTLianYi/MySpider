//
// Created by lianyi on 18-2-28.
//

#ifndef MYSPIDER_LOG_H
#define MYSPIDER_LOG_H
#include <errno.h>
/* External variables */
extern int h_errno;    /* defined by BIND for DNS errors */
extern char **environ; /* defined by libc */
class Log {
    public:
    static void unix_error(const char *msg);
    static void posix_error(const int code, char *msg);
    static void dns_error(const char *msg);
    static void app_error(const char *msg);
};


#endif //MYSPIDER_LOG_H
