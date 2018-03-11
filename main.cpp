#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include<iostream>
#include "TaskQueue.h"
#include "TaskQueueSelect.h"
using namespace std;
int main() {

    TaskQueueSelect m_TaskQueueSelect;
    m_TaskQueueSelect.init("http://en.cppreference.com/w/");

    m_TaskQueueSelect.Run();
    return 0;
}


//#include <iostream>
//#include <evdns.h>
//#include <string.h>
//#include<arpa/inet.h>
//#include <zconf.h>
//#include "csapp.h"
//
//#include <boost/iostreams/device/array.hpp>
//#include <boost/iostreams/device/back_inserter.hpp>
//#include <boost/iostreams/filtering_stream.hpp>
//#include <boost/iostreams/filter/zlib.hpp>
//#include <vector>
//#include <string>
//#include <iostream>
//#include<bits/stdc++.h>
////using namespace boost::iostreams;
//
//#define MODE S_IRUSR|S_IWUSR|S_IXUSR
//using namespace std;
//typedef struct Surl {
//    char  *url;
//    int    level;
//    int    type;
//} Surl;
//
//typedef struct Url {
//    char *domain; //域名
//    char *path;
//    int  port;
//    char *ip;
//    int  level;
//} Url;
//#include <stdio.h>
//#include<stdlib.h>
//#include <string.h>
//#include <zlib.h>
//
//int gzDecompress(const char *src, int srcLen, const char *dst, int dstLen){
//    z_stream strm;
//    strm.zalloc=NULL;
//    strm.zfree=NULL;
//    strm.opaque=NULL;
//
//    strm.avail_in = srcLen;
//    strm.avail_out = dstLen;
//    strm.next_in = (Bytef *)src;
//    strm.next_out = (Bytef *)dst;
//
//    int err=-1, ret=-1;
//    err = inflateInit2(&strm, MAX_WBITS+16);
//    if (err == Z_OK){
//        err = inflate(&strm, Z_FINISH);
//        if (err == Z_STREAM_END){
//            ret = strm.total_out;
//        }
//        else{
//            inflateEnd(&strm);
//            return err;
//        }
//    }
//    else{
//        inflateEnd(&strm);
//        return err;
//    }
//    inflateEnd(&strm);
//    return err;
//}
//static void dns_callback(int result, char type, int count, int ttl, void *addresses, void *arg)
//{
//    Url * ourl = (Url *)arg;
//    struct in_addr *addrs = (in_addr *)addresses;
//
//    if (result != DNS_ERR_NONE || count == 0) {
//        //SPIDER_LOG(SPIDER_LEVEL_WARN, "Dns resolve fail: %s", ourl->domain);
//    } else {
//        char * ip = inet_ntoa(addrs[0]);
//        // SPIDER_LOG(SPIDER_LEVEL_DEBUG, "Dns resolve OK: %s -> %s", ourl->domain, ip);
//        //  host_ip_map[ourl->domain] = strdup(ip);
//        ourl->ip = strdup(ip);
//        //  push_ourlqueue(ourl);
//    }
//    event_loopexit(NULL); // not safe for multithreads
//}

//#include <boost/lexical_cast.hpp>
//#include <iostream>
//#include<boost/regex.hpp>
//#include<bits/stdc++.h>
//
//static const std::string protocol = "(?:(mailto|ssh|ftp|https?)://)?";
////static const std::string hostname = "(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+(?:com|net|edu|biz|gov|org|in(?:t|fo)|(?-i:[a-z][a-z]))";
//
//static const std::string hostname = "(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+(?:com|net|edu|biz|gov|org|in(?:t|fo))";
//static const std::string ip = "(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])";
//static const std::string port = "(?::(\\d{1,5}))?";
//static const std::string path = "(/[^\">'?]*)?";
//static const std::string pattern = protocol + "((?:" + hostname + "|" + ip + "))" + port + path;
//static const std::string path1 = "(/[^\">'?]*)+";
//static const std::string pattern1 = "(href=\")"+path1;
//
////using namespace std;
//template<typename charT>
//inline bool boost_match(const charT* pattern,const charT* text,unsigned int flags=boost::regex::normal,boost::match_results<const charT*>* result=NULL)
//{
//    boost::basic_regex<charT,boost::regex_traits<charT> > expression(pattern,flags);
//    if(NULL==result)
//        return boost::regex_match(text,expression);
//    return boost::regex_match(text,*result,expression);
//}
//
//template<typename charT>
//inline bool boost_search(const charT* pattern,const charT* text,unsigned int flags=boost::regex::normal,boost::match_results<const charT*>* result=NULL)
//{
//    boost::basic_regex<charT,boost::regex_traits<charT> > expression(pattern,flags);
//    if(NULL==result)
//        return boost::regex_search(text,expression);
//    return boost::regex_search(text,*result,expression);
//}
//
//int main() {
//
//    Url *ourl=new Url;
//    event_base * base = event_init();
//    evdns_init();
//    string ym="en.cppreference.com";
//    //href="http://v.baidu.com"
//    // 58.217.200.113
//    //evdns_resolve_ipv4("zhidao.baidu.com", 0, dns_callback, ourl);
//    evdns_resolve_ipv4(ym.c_str(), 0, dns_callback, ourl);
//    event_dispatch();
//    event_base_free(base);
//
//    cout<<ourl->ip<<endl;
//
//    int con= socket(AF_INET,SOCK_STREAM,0);
//    if(con==-1){
//        cout<<"sockcreate error"<<endl;
//        return 0;
//    }
//
//    struct sockaddr_in  addr;
//    bzero(&addr,sizeof(addr));
//    addr.sin_family = AF_INET;
//    addr.sin_port=htons(80);
//
//    inet_pton(AF_INET,ourl->ip,&addr.sin_addr);
//    if(connect(con,(struct sockaddr*)&addr,sizeof(addr)) ==-1 ){
//        cout<<"connect error"<<endl;
//    }
//    //char *sdbuf="GET /img/bd_logo.png HTTP/1.0\r\n\r\n";
//    // char *sdbuf="GET / HTTP/1.0\r\n\r\n";
//    //  "Accept-Encoding: gzip, deflate\r\n"
//    //char *sdbuf="GET https://www.baidu.com/ HTTP/1.1\r\n"
////    char *sdbuf="GET https://www.baidu.com/ HTTP/1.1\r\n"
////     "Host: www.baidu.com\r\n\r\n";
//    //"Connection: keep-alive\r\n"
//    // "Pragma: no-cache\r\n"
//    // "Cache-Control: no-cache\r\n\r\n";
//    //   "Host: www.baidu.com\r\n\r\n";
////    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:58.0) Gecko/20100101 Firefox/58.0\r\n"
////    "Accept: */*\r\n"
////"Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\n"
////"Referer: https://www.baidu.com/\r\n"
////"Connection: keep-alive\r\n\r\n";
//
//    string url="http://en.cppreference.com/w/";
//    char sdbuf[1024];
//    sprintf(sdbuf,"GET %s HTTP/1.1\r\n"
//            "Host: %s\r\n\r\n",url.c_str(),ym.c_str());
//    int len=url.size()+ym.size()+15+10;
//    sdbuf[len]='\0';
//    cout<<sdbuf<<endl;
//    if(send(con,sdbuf,strlen(sdbuf),0)==-1){
//        cout<<"send error"<<endl;
//        return 0;
//    }
//    char buf[2048];
//    ssize_t rn=0;
//    rio_t rio;
//    Rio_readinitb(&rio,con);
//    while( (rn=Rio_readlineb(&rio,buf,2048))>0){
//
//
//        cout<<buf;
//
//        if(buf[0]=='\r' && buf[1]=='\n'){
//            break;
//        }
//    }
//
//    int fd2;
//
//    fd2=open("/home/lianyi/pp32.html",O_RDWR|O_CREAT|O_TRUNC,MODE);
//    if(fd2==-1){
//        cout<<"crfile error"<<endl;
//        return 0;
//    }
//
////    while( (rn= recv(con,buf,1024,0)) >0){
////        //int st=ungzip(buf,rn,resbuf);
////        //cout<<st<<endl;
////        //cout<<resbuf<<endl;
////       cout<<buf;
////        boost::smatch sm;
////        boost::regex reg(pattern);
////        //char *p=buf;
////        string tmp=string(buf,buf+rn);
////        boost::sregex_iterator rit(tmp.begin(),tmp.end(),reg);
////        boost::sregex_iterator rend;
////        for(;rit!=rend;rit++){
////            //cout<<rit->str()<<endl;
////           // cout<<rit->str(1)<<" "<<rit->str(2)<<" "<<rit->str(3)<<" "<<rit->str(4)<<endl;
////        }
////        write(fd2,buf,rn);
////    }
//    // ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n);
//    int fdr=open("/home/lianyi/p1.png",O_RDWR);
//    while((rn=rio_readnb(&rio,buf,1024))>0) {
//        cout<<buf;
//
//        boost::smatch sm;
//        boost::regex reg(pattern);
//        string tmp=string(buf,buf+rn);
//        boost::sregex_iterator rit(tmp.begin(),tmp.end(),reg);
//        boost::sregex_iterator rend;
//        for(;rit!=rend;rit++){
//            cout<<rit->str()<<endl;
//            // cout<<rit->str(1)<<" "<<rit->str(2)<<" "<<rit->str(3)<<" "<<rit->str(4)<<endl;
//        }
//
//        write(fd2,buf,rn);
//    }
//
//
//    return 0;
//}
//
//
