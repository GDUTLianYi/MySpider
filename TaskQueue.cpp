//
// Created by lianyi on 18-2-25.
//

#include "TaskQueue.h"
#include "CSocket.h"
#include"Http.h"
#include "Log.h"
#include "Rio.h"
#define MODE S_IRUSR|S_IWUSR|S_IXUSR


int CreateDir(const char *sPathName)
{
    char DirName[256];
    strcpy(DirName, sPathName);
    int i,len = strlen(DirName);
    if(DirName[len-1]!='/')
        strcat(DirName, "/");

    len = strlen(DirName);

    for(i=1; i<len; i++)
    {
        if(DirName[i]=='/')
        {
            DirName[i] = 0;
            if( access(DirName, NULL)!=0 )
            {
                if(mkdir(DirName, 0755)==-1)
                {
                    perror("mkdir error");
                    return -1;
                }
            }
            DirName[i] = '/';
        }
    }

    return 0;
}
int TaskQueue::Run(){

    int i=0;
    while(!m_UrlParse.Empty()){
        i++; if(i>100){
            std::cout<<"第几个请求 "<<i<<std::endl;
            break;
        }
        struct Url t=m_UrlParse.pop();
        int con=CSocket::Open_clientfd(t.ip.c_str(),t.port);
        if(con==-1){
            continue;
        }
        char sdbuf[1024];
        Http::Form_Get(sdbuf,t);
        if(send(con,sdbuf,strlen(sdbuf),0)==-1){
            Log::unix_error("send get error");
            continue;
        }
        std::cout<<sdbuf;
        Rio m_rio;
        rio_t rio;
        char buf[RIO_BUFSIZE];
        m_rio.readinitb(&rio,con);
        ssize_t rn=0;
        int state=-1;
        char pro[20],res[20];
        while( (rn=m_rio.readlineb(&rio,buf,RIO_BUFSIZE))>0){
            std::cout<<buf;
            if(state==-1) {
                sscanf(buf, "%s %d %s",pro,&state,res);
            }
            if(buf[0]=='\r' && buf[1]=='\n'){
                break;
            }
        }

        if(state!=200){
            continue;
        }

        std::string DownLoadPath="/home/lianyi/MySpideDownLoad";
        std::string::size_type pos=t.path.find_last_of('/');


        std::string SavePath=DownLoadPath+t.path;
        std::string dir=DownLoadPath;
        std::string FileName("");
        if(pos!=std::string::npos){
            dir+=t.path.substr(0,pos+1);
            FileName=t.path.substr(pos+1);
        }
        if(t.path.empty()||t.path=="/"||FileName.empty()) {
            if(SavePath.back()!='/'){
                SavePath.push_back('/');
            }
            SavePath+=t.domain+".html";
        }else {

            if (FileName.find('.') == std::string::npos) {
                SavePath+=".html";
            }
        }
        CreateDir(dir.c_str());
//            if(mkdir(dir.c_str(),0775)==-1){
//                std::cout<<"mkdir failure"<<std::endl;
//            }
        int Fd_SaveFile=open(SavePath.c_str(),O_RDWR|O_CREAT|O_TRUNC,MODE);


        if(Fd_SaveFile==-1){
            close(con);
            Log::unix_error("create open file error");
            continue;
        }
       // int i=0;
        while((rn=m_rio.readnb(&rio,buf,RIO_BUFSIZE))>0) {
           // std::cout<<i++<<std::endl;
            m_UrlParse.GetUrl(buf,rn,t.domain,t.port);
          //  std::cout<<"hehe"<<std::endl;
            write(Fd_SaveFile,buf,rn);
        }

        close(con);
        close(Fd_SaveFile);
    }


}

int TaskQueue::init(std::string url){
    struct Url u;
    std::tuple<std::string,std::string,std::string> res=m_UrlParse.ToParseUrl(url);
    u.url=url;
    u.domain= std::get<0>(res);
    if(!std::get<1>(res).empty())
    u.port= std::stoi(std::get<1>(res));
    else
    u.port=80;
    u.path=std::get<2>(res);
    u.state=0;
    u.dep=0;
    if(!m_UrlParse.AddUrl(u)){
        Log::unix_error("init erroe");
    }
}