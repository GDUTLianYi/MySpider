//
// Created by lianyi on 18-2-28.
//

#include "CSocket.h"
#include<arpa/inet.h>
#include <zconf.h>

pthread_mutex_t mutex_Que_UrlBeforeConnect = PTHREAD_MUTEX_INITIALIZER;
std::queue<Url> Que_UrlBeforeConnect;
extern int FinshSign;
extern std::queue<Url> Que_UrlBeforeDns;

typedef std::unordered_set<struct Url, UrlKey, UrlCmp> urlSet;
extern std::unordered_map<std::size_t, urlSet> ipMap;
extern pthread_mutex_t mutex_ipMap;

static std::size_t GetHashcode(std::string s) {
    return std::hash<std::string>{}(s);
}

int CSocket::open_clientfd(const char *ip, int port) {
    int clientfd;
//        struct hostent *hp;
    struct sockaddr_in serveraddr;
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
//        bcopy((char *)hp->h_addr_list[0],
//              (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serveraddr.sin_addr);
    if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
        return -1;
    return clientfd;
}

int CSocket::Open_clientfd(const char *ip, int port) {
    int rc;

    if ((rc = open_clientfd(ip, port)) < 0) {
        if (rc == -1)
            Log::unix_error("Open_clientfd Unix error");
    }
    return rc;
}


void *ThreadSocketFunc(void *argc) {


    int emp = 0;
    CSocket socketFactory;

    fd_set rset, wset;
    FD_ZERO(&wset);
    FD_ZERO(&rset);
    int maxfd = 0;
    struct Url tmpurl[100];
    int lisfd[100];
    memset(lisfd, -1, sizeof(lisfd));

    for (; FinshSign != 1;) {


        if (emp) {
            std::cout << "con sleep() " << std::endl;
            sleep(5);
            emp = 0;
        }
        if (1) {

            // std::cout<<"apply mutex_Que_UrlBeforeConnect"<<std::endl;
            MutexRAII<pthread_mutex_t> lcks(mutex_Que_UrlBeforeConnect);
            // std::cout<<"apply mutex_Que_UrlBeforeConnect OK "<<std::endl;

            for (int i = 0; i < 20; i++) {
                std::cout<<i << "  con activi" << Que_UrlBeforeConnect.size() << std::endl;

                if (lisfd[i] == -1) {

                    std::cout<<" mutex_Que_UrlBeforeConnect size "<<Que_UrlBeforeConnect.size()<<std::endl;


                    if (1) {

                        if (Que_UrlBeforeConnect.empty()) {
                            emp = 1;
                            break;
                        } else {
                            tmpurl[i] = Que_UrlBeforeConnect.front();
                            Que_UrlBeforeConnect.pop();
                        }
                    }

                    int clientfd;
                    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                        Log::unix_error("create socket error");
                        continue;
                    }
                    tmpurl[i].fd = clientfd;
                    lisfd[i] = 0;

                    int flags = fcntl(clientfd, F_GETFD, 0);
                    fcntl(clientfd, F_SETFD, flags | O_NONBLOCK);

                    struct sockaddr_in serveraddr;
                    bzero(&serveraddr, sizeof(serveraddr));
                    serveraddr.sin_family = AF_INET;
                    serveraddr.sin_port = htons(tmpurl[i].port);
                    inet_pton(AF_INET, tmpurl[i].ip.c_str(), &serveraddr.sin_addr);

                    if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
                        if (errno != EINPROGRESS) {
                            Log::unix_error("noblock connect error");
                            close(clientfd);
                            lisfd[i] = -1;
                            continue;
                        } else {
                            if (clientfd > maxfd) {
                                maxfd = clientfd;
                            }
                            /*
                             * 设置可读可写
                             */
                            FD_SET(clientfd, &rset);
                            FD_SET(clientfd, &wset);
                        }
                    } else {

                        lisfd[i] = -1;
                        if (1) {
                            size_t domainHash = GetHashcode(tmpurl[i].domain);

                            std::cout<<"apply mutex_ipMap"<<std::endl;
                            MutexRAII<pthread_mutex_t> lck(mutex_ipMap);
                              std::cout<<"apply mutex_ipMap OK "<<std::endl;


                            ipMap[domainHash].insert(tmpurl[i]);
                            for (auto &i:ipMap) {
                                std::cout << "ipMap " << i.second.size() << std::endl;
                            }
                            std::cout << std::endl;
                            if (ipMap[domainHash].size() > 100) {
                                emp = 1;
                            }
                            // std::cout<<"add Url  :domain= "<<url.domain <<"  "<<url.url<<std::endl;
                        }

                    }


                }
                std::cout << "con activi do and Que_UrlBeforeConnect size=" << Que_UrlBeforeConnect.size()
                          << std::endl<< std::endl;
            }

        }

        fd_set rs, ws;
        rs = rset, ws = wset;
        int n;
        struct timeval selecttimeout;
        selecttimeout.tv_sec = 3;
        n = select(maxfd + 1, &rs, &ws, NULL, &selecttimeout);

        for (int i = 0; i < 20; i++) {

            if (lisfd[i] == -1) {
                continue;
            }

            int tmpfd = tmpurl[i].fd;

            if ((FD_ISSET(tmpfd, &rs) || FD_ISSET(tmpfd, &ws))) {
                FD_CLR(tmpfd, &wset);
                FD_CLR(tmpfd, &rset);
                lisfd[i] = -1;
                /*
                 * 清除可读可写
                 */

                int error;
                n = sizeof(error);
                if (getsockopt(tmpfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *) &n) < 0 || error != 0) {
                    close(tmpfd);
                    Log::unix_error("nonblock connect failure");
                    continue;
                }

                if (1) {
                    size_t domainHash = GetHashcode(tmpurl[i].domain);

                    // std::cout<<"apply mutex_ipMap"<<std::endl;
                    MutexRAII<pthread_mutex_t> lck(mutex_ipMap);
                    //  std::cout<<"apply mutex_ipMap OK "<<std::endl;


                    ipMap[domainHash].insert(tmpurl[i]);
                    for (auto &i:ipMap) {
                        std::cout << "ipMap " << i.second.size() << std::endl;
                    }
                    std::cout << std::endl;
                    if (ipMap[domainHash].size() > 150) {
                        emp = 1;
                    }
                    // std::cout<<"add Url  :domain= "<<url.domain <<"  "<<url.url<<std::endl;
                }
            }

        }
    }
}