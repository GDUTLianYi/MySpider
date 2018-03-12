//
// Created by lianyi on 18-2-27.
//

#include "UrlParse.h"

void UrlParse::GetUrl(const char *buf,int len,std::string domain,int port){

    std::string str=std::string(buf,buf+len);
    boost::regex reg_url(pattern_url,boost::regex_constants::icase);
    boost::sregex_iterator  rit(str.begin(),str.end(),reg_url);
    boost::sregex_iterator  rend;

    for(;rit!=rend;rit++){
        struct Url tmp;
        tmp.url=rit->str();
        tmp.domain=rit->str(2);
        if(!rit->str(3).empty())
            tmp.port=stoi(rit->str(3));
        else
            tmp.port=80;
        tmp.path=rit->str(4);
        tmp.dep=0;
        tmp.state=0;
        AddUrl(tmp);
    }
    boost::regex reg_href(pattern_href,boost::regex_constants::icase);
    boost::sregex_iterator  rit_href(str.begin(),str.end(),reg_href);
    for(;rit_href!=rend;rit_href++){
        struct Url tmp;
        tmp.url="http://"+domain+rit_href->str(2);
        tmp.domain=domain;
        tmp.port=port;
        tmp.path=rit_href->str(2);
        tmp.dep=0;
        tmp.state=0;
        AddUrl(tmp);
    }
}


int UrlParse::AddUrl(struct Url url){
    if(Set_Url.count(url.url)){
        return  0;
    }
    Set_Url.insert(url.url);
    char *ip= DNS::GetHostByName(url.domain);
    if(ip==NULL){
        return -1;
    }
    url.ip=ip;
    free(ip);

    Que_Url.push(url);
    return 1;
}

struct Url UrlParse::pop(){
    struct Url u=Que_Url.front();
    Que_Url.pop();
    return u;
}
bool UrlParse::Empty(){
    return Que_Url.empty();
}

std::tuple<std::string,std::string,std::string> UrlParse::ToParseUrl(std::string url){

    boost::regex reg(pattern_url);
    std::string domain(""),port(""),path("");
    boost::smatch sm;
    if(boost::regex_match(url,sm,reg)){
        domain=sm[2];
        port=sm[3];
        path=sm[4];
    }
    return std::make_tuple(domain,port,path);
}