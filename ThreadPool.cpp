//
// Created by lianyi on 18-3-20.
//

#include "ThreadPool.h"

typedef std::unordered_set<struct Url, UrlKey, UrlCmp> urlSet;
extern std::unordered_map<std::size_t, urlSet> ipMap;
extern std::queue<RegexPackage> Que_RegexFile;
extern pthread_mutex_t mutex_ipMap;
extern pthread_mutex_t mutex_Que_RegexFile;
extern pthread_mutex_t mutex_Set_Url;
extern pthread_mutex_t mutex_threadNum;
extern int threadNum;
extern int FinshSign;

void *SpiderThreadFunc(void *agrc) {


//    int timeout = 0;
//    size_t ac = *((std::size_t *) agrc);
//    free(agrc);
//    size_t hashdomain = ac;
//    std::cout << "in thread  " << hashdomain << std::endl;
    int reconnect = 0;
    std::string ip;
    int port;
    char sdbuf[9192];
    CSocket socketFactory;
    Http httpHelper;
    Rio m_rio;
    Url tmpurl;
    int gt, emp = 0;

    for (; FinshSign != 1;) {
        gt = 0;
        if (emp == 1) {
            sleep(5);

        } else {

        }

        if (1) {

            MutexRAII<pthread_mutex_t> lock(mutex_ipMap);
            if (ipMap.empty()) {
                emp = 1;
                continue;
                // pthread_exit(NULL);
            }
            if (((ipMap.begin())->second).empty()) {

                ipMap.erase(ipMap.begin());
                emp = 1;
                continue;

            }

            tmpurl = *(((ipMap.begin())->second).begin());
            (ipMap.begin())->second.erase(((ipMap.begin())->second).begin());
            if (((ipMap.begin())->second).empty()) {
                ipMap.erase(ipMap.begin());
            }
        }


//        port = tmpurl.port;
//        ip = tmpurl.ip;
//        // std::cout<<"create socket"<<std::endl;
//        int clientfd = socketFactory.open_clientfd(ip.c_str(), port);
//        //std::cout<<"create socket ok  "<<clientfd <<std::endl;
//
//        if (clientfd == -1) {
//            MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
//            std::cout << "threadNum " << threadNum-- << std::endl;
//
//
//            continue;
//        }

        int clientfd=tmpurl.fd;

        httpHelper.Form_Get(sdbuf, tmpurl);
        std::cout << "want to send :" << sdbuf << std::endl;
        if (send(clientfd, sdbuf, strlen(sdbuf), 0) == -1) {
            Log::unix_error("send get error");
            MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
            std::cout << "threadNum " << threadNum-- << std::endl;
            close(clientfd);

            continue;

        } else {

        }


        rio_t rio;
        m_rio.readinitb(&rio, clientfd);


        ssize_t rn;
        int state = -1;
        char pro[20], res[20];

        while ((rn = m_rio.readlineb(&rio, sdbuf, 8196)) > 0) {
            // std::cout << sdbuf;
            if (rn < 0) {
                close(clientfd);
                gt = 1;
                break;

            }
            if (state == -1) {
                sscanf(sdbuf, "%s %d %s", pro, &state, res);
            }
            if (sdbuf[0] == '\r' && sdbuf[1] == '\n') {
                break;
            }
        }

        if (gt || state != 200) {

            close(clientfd);

            continue;
        }
        std::string DirPath, FilePath;
        FileHelper fileHelper;
        fileHelper.getDirAndFileName(tmpurl, DirPath, FilePath);
        fileHelper.CreateDir(DirPath.c_str());
        int Fd_SaveFile = open(FilePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, MODE);
        if (Fd_SaveFile == -1) {
            Log::unix_error("create open file error");
            close(clientfd);
            continue;
        }

        //std::cout << FilePath << std::endl;
        while ((rn = m_rio.readnb(&rio, sdbuf, 8196)) > 0) {
            // std::cout<<i++<<std::endl;
            if (rn < 0) {
                close(clientfd);

                gt = 1;
                break;
            }
           // std::cout << " " << rn << std::endl;
            write(Fd_SaveFile, sdbuf, rn);
        }

        close(Fd_SaveFile);
        close(clientfd);
        if (gt == 1) {
            continue;
        }
        do {
            //std::cout << "apply mutex_Que_RegexFile" << std::endl;
            MutexRAII<pthread_mutex_t> lockf(mutex_Que_RegexFile);
            //std::cout << "apply mutex_Que_RegexFile OK " << std::endl;
            // std::cout<<"add FIlE :: "<<FilePath<<std::endl;
            Que_RegexFile.push(RegexPackage{FilePath, tmpurl.domain, tmpurl.port});
        } while (0);
    }
    return NULL;
}