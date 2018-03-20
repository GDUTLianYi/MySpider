//
// Created by lianyi on 18-2-24.
//
#include "MutexRAII.h"
#include "DNS.h"
extern int FinshSign;
pthread_mutex_t mutex_Map_HostToIp=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Que_UrlBeforeDns=PTHREAD_MUTEX_INITIALIZER ;
extern pthread_mutex_t mutex_Que_UrlBeforeConnect;
extern  std::queue<Url> Que_UrlBeforeConnect;
std::queue<Url> Que_UrlBeforeDns;
std::map<std::string,std::string>Map_HostToIp;

 char *myDNS::GetHostByName(std::string hostname){

    MutexRAII<pthread_mutex_t>lockf(mutex_Map_HostToIp);

    if(Map_HostToIp.count(hostname)){
        if(Map_HostToIp[hostname].empty()){
            return NULL;
        }
        //  return strdup(Map_HostToIp[hostname].c_str());
        return strdup(Map_HostToIp[hostname].c_str());
    }
    char * ptr,**pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;

    hptr=gethostbyname(hostname.c_str());
    if(hptr==NULL){
        Log::unix_error("gethostbyName failure");
        std::cout<<hostname<<std::endl;
        Map_HostToIp[hostname]="";
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


void * ThreadDnsFunc(void *argc){
    int emp=0;
    myDNS dnshelper; Url tmpurl;

    for(;FinshSign!=1;) {
        if(emp){
            sleep(5);
            emp=0;
        }
        if(1) {

             // std::cout<<"apply mutex_Que_UrlBeforeDns"<<std::endl;
            MutexRAII<pthread_mutex_t> lcks(mutex_Que_UrlBeforeDns);
           // std::cout<<"apply mutex_Que_UrlBeforeDns OK "<<std::endl;
            if (Que_UrlBeforeDns.empty()) {
                emp=1;
                continue;
            } else {

                tmpurl=Que_UrlBeforeDns.front();
                Que_UrlBeforeDns.pop();

            }
        }
        std::cout<<"begin dns"<<std::endl;
        char *ip=dnshelper.GetHostByName(tmpurl.domain);
        std::cout<<"end dns"<<std::endl;

        if (ip == NULL) {
            continue;
        }
        tmpurl.ip = ip;
        free(ip);

        if(1){
            //std::cout<<"apply mutex_Que_UrlBeforeConnect"<<std::endl;
            MutexRAII<pthread_mutex_t> lcks(mutex_Que_UrlBeforeConnect);
           // std::cout<<"apply mutex_Que_UrlBeforeConnect OK "<<std::endl;
            Que_UrlBeforeConnect.push(tmpurl);
            std::cout<<"Que_UrlBeforeConnect size= "<<Que_UrlBeforeConnect.size()<<std::endl;
            if(Que_UrlBeforeConnect.size()>500){
                emp=1;
            }
        }
    }
}
