//
// Created by lianyi on 18-2-27.
//

#ifndef MYSPIDER_URLPARSE_H
#define MYSPIDER_URLPARSE_H


#include<bits/stdc++.h>
#include"DNS.h"
#include<string>
#include<queue>
#include<map>
#include<set>
#include"CSocket.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include<boost/regex.hpp>
#include"MutexRAII.h"
#include"Http.h"
#include"Rio.h"


static const std::string protocol = "(?:(mailto|ssh|ftp|https?)://)?";
//static const std::string hostname = "(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+(?:com|net|edu|biz|gov|org|in(?:t|fo)|(?-i:[a-z][a-z]))";
static const std::string hostname = "(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+(?:com|net|edu|biz|gov|org|in(?:t|fo))";
static const std::string ip = "(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])";
static const std::string port = "(?::(\\d{1,5}))?";
static const std::string path = "(/[^\">'?)]*)?";
static const std::string pattern_url = protocol + "(" + hostname + "|" + ip + ")" + port + path;
static const std::string path_href = "(/[^\">'?)]*)+";
static const std::string pattern_href = "(href=\")"+path_href;

struct Url{
    std::string url;
    std::string domain;
    std::string ip;
    std::string path;
    int port;
    int dep;
    int state;
};
class UrlKey{
public:
    std::size_t operator()(const Url &rc) const
    {
        return std::hash<std::string>{}(rc.url);
    }
};
class UrlCmp{
public:
    bool operator()(const Url &rc1,const Url &rc2)const{
        return rc1.url==rc2.url;
    }
};

struct RegexPackage{
    std::string file;
    std::string domain;
    int port;
};



class UrlParse {

    public:
    void Init(std::string url);

};


#endif //MYSPIDER_URLPARSE_H
