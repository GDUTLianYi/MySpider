//
// Created by lianyi on 18-2-24.
//

#include "DNS.h"
std::map<std::string,std::string>DNS::Map_HostToIp;

 char *DNS::GetHostByName(std::string hostname){

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
