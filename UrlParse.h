//
// Created by lianyi on 18-2-27.
//

#ifndef MYSPIDER_URLPARSE_H
#define MYSPIDER_URLPARSE_H

#include"DNS.h"
#include<string>
#include<queue>
#include<map>
#include<set>

#include <boost/lexical_cast.hpp>
#include <iostream>
#include<boost/regex.hpp>
#include<bits/stdc++.h>

static const std::string protocol = "(?:(mailto|ssh|ftp|https?)://)?";
//static const std::string hostname = "(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+(?:com|net|edu|biz|gov|org|in(?:t|fo)|(?-i:[a-z][a-z]))";

static const std::string hostname = "(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+(?:com|net|edu|biz|gov|org|in(?:t|fo))";
static const std::string ip = "(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])";
static const std::string port = "(?::(\\d{1,5}))?";
static const std::string path = "(/[^\">'?)]*)?";
static const std::string pattern_url = protocol + "((?:" + hostname + "|" + ip + "))" + port + path;
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

class UrlParse {

    public:
    void GetUrl(const char *buf,int len,std::string domain="",int port=80);
    std::queue<Url> Que_Url;
    std::set<std::string> Set_Url;
    int AddUrl(struct Url url);
    struct Url pop();
    bool Empty();
    std::tuple<std::string,std::string,std::string> ToParseUrl(std::string url);
};


#endif //MYSPIDER_URLPARSE_H
