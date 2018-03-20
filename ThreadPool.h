//
// Created by lianyi on 18-3-20.
//
#include"UrlParse.h"
#ifndef MYSPIDER4_THREADPOOL_H
#define MYSPIDER4_THREADPOOL_H
#include<queue>
#include"FileHelper.h"
#include"Http.h"
#include"CSocket.h"
#include"MutexRAII.h"

#define MODE S_IRUSR|S_IWUSR|S_IXUSR

void *SpiderThreadFunc(void *agrc);
class ThreadPool {

public:
    std::vector<pthread_t> v;

    explicit ThreadPool(int _n):v(_n){};

    void Init(int n){

        for(int i=0;i<n;i++){
            pthread_t  tid;
            int terror=pthread_create(&tid,NULL,SpiderThreadFunc,NULL);
            if(terror==0){
                v.push_back(tid);
                  std::cout<<"create thread success "<<tid<<std::endl;
            }else{
                std::cout<<"create thread error "<<tid<<std::endl;
            }
        }
    }
};


#endif //MYSPIDER4_THREADPOOL_H
