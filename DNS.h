//
// Created by lianyi on 18-2-24.
//

#ifndef MYSPIDER_DNS_H
#define MYSPIDER_DNS_H

#include<string>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <map>
#include <queue>
#include <zconf.h>


class myDNS {


public:
    myDNS(){};
     char *GetHostByName(std::string hostname);
};


#endif //MYSPIDER_DNS_H
