//
// Created by lianyi on 18-2-28.
//

#ifndef MYSPIDER_CSOCKET_H
#define MYSPIDER_CSOCKET_H


#include <netinet/in.h>
#include<string.h>
#include"UrlParse.h"
class CSocket {
  public:
     int open_clientfd(const char *ip, int port);
     int Open_clientfd(const char *ip, int port=80);
};


#endif //MYSPIDER_CSOCKET_H
