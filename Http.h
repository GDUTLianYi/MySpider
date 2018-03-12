//
// Created by lianyi on 18-2-28.
//

#ifndef MYSPIDER_HTTP_H
#define MYSPIDER_HTTP_H


#include <cstdio>
#include "UrlParse.h"

class Http {
public:
    static void Form_Get(char *sdbuf,const struct Url& u);
};


#endif //MYSPIDER_HTTP_H
