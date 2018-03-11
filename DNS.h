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
    static char *GetHostByName(std::string hostname){

        if(Map_HostToIp.count(hostname)){
          //  return strdup(Map_HostToIp[hostname].c_str());
            return strdup(Map_HostToIp[hostname].c_str());
        }
        char * ptr,**pptr;
        char str[INET_ADDRSTRLEN];
        struct hostent *hptr;

        hptr=gethostbyname(hostname.c_str());
        if(hptr==NULL){
            printf("DNS failure\n");
            return NULL;
        }

        pptr=hptr->h_addr_list;
        if(*pptr!=NULL && hptr->h_addrtype==AF_INET){
            char *p=strdup(inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str))) ;
            std::string ip=p;
            Map_HostToIp[hostname]=ip;
            return p;
        }
        return NULL;
    }

};


#endif //MYSPIDER_DNS_H
