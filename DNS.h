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


class DNS {

    static std::map<std::string,std::string>Map_HostToIp;

public:
    static char *GetHostByName(std::string hostname);

};


#endif //MYSPIDER_DNS_H
