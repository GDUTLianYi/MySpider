//
// Created by lianyi on 18-2-28.
//

#include "Http.h"
 void Http::Form_Get(char *sdbuf,const struct Url& u){

    sprintf(sdbuf,"GET %s HTTP/1.1\r\n"
            "Host: %s\r\n\r\n",u.url.c_str(),u.domain.c_str());
    int len=u.url.size()+u.domain.size()+15+10;
    sdbuf[len]='\0';

}